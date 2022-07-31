#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include "math.h"
#define N 800 // 問題サイズ

int main(argc, argv)
int argc;
char **argv;
{

    float ans[N][N] = {};
    float A[N][N] = {};
    float B[N][N] = {};
    double start, matrixTime, matrixEndTime, endTime;
    unsigned long long operatorCount = 0;
    float operatorCountSum = 0;
    start = MPI_Wtime();

    int myrank; // 自ノード番号を入れる変数
    int size;   // The number of process,使用するノード数を入れる変数
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    matrixTime = MPI_Wtime();

    // 行列・ベクトル初期化
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = 2.0f;
            B[i][j] = i * j;
        }
    }
    matrixTime = MPI_Wtime();

    for (int i = N / size * myrank; i < N / size * (myrank + 1); i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                ans[i][j] += A[i][k] * B[k][j];
                operatorCount++;
            }
        }
    }
    matrixEndTime = MPI_Wtime();

    // printf("for文内の時間は%g秒\n", matrixEndTime - matrixTime);

    MPI_Reduce(            // ノード間で集計する関数
        &myrank,           // 自ノードが持つデータが入っている変数
        &operatorCountSum, // 集計結果を入れる変数
        1,                 // 送受信するデータ個数
        MPI_INT,           // 通信するデータのデータ型
        MPI_SUM,           // 集計の種類
        0,                 // 集計結果を集めるノード
        MPI_COMM_WORLD     // 集計するノードのグループ
    );

    MPI_Barrier(MPI_COMM_WORLD);
    endTime = MPI_Wtime();
    // printf("Number of processes size : %d \n", size);
    // printf("My rank Number: %d\n", myrank);
    printf("Nのサイズ%d\n", N);
    printf("ノード数%d\n", size);
    printf("主要実行時間%g秒\n", matrixEndTime - matrixTime);
    printf("その他%g秒\n", (endTime - start) - (matrixEndTime - matrixTime));
    printf("実行回数%llu:\n", operatorCount);
    printf("全体実行時間%g秒\n", endTime - start);
    MPI_Finalize();
    return 0;
}

/*
MPI実行コマンド
mpicc hoge.c
mpirun -np X ./a.out
実行時にノードが足りないときは
mpirun --oversubscribe -np X ./a.out

X：ノード数
*/

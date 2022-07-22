#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include "math.h"
#include <omp.h>
#include <time.h>
#define N 600 // 問題サイズ

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

    // int id = N / size;
    // int istart = 1 + myrank * id;
    // int iend = (myrank + 1) * id;
    // if (myrank == size - 1)
    //     iend = N;

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
        for (int j = 0; j < N; j += 2)
        {
            ans[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                ans[i][j] += A[i][k] * B[k][j];
                ans[i][j] += A[i][k] * B[k][j + 1];
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
    printf("Number of processes size : %d \n", size);
    printf("My rank Number: %d\n", myrank);
    printf("実行回数%llu:\n", operatorCount);
    printf("%g秒\n", endTime - start);
    MPI_Finalize();
    return 0;
}

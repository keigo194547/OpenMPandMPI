#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <omp.h>
#include <time.h>
#define N 800 // 問題サイズ

int main(argc, argv)
int argc;
char **argv;
{

  float ans = 0;
  float A[N][N] = {};
  float B[N][N] = {};
  unsigned long long operatorCount = 0;
  double st, ed, mainST, mainED;
  st = omp_get_wtime(); // 計測開始

  int myid;
  // 行列・ベクトル初期化
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      A[i][j] = 2.0f;
      B[i][j] = i * j;
    }
  }

  mainST = omp_get_wtime();

#pragma omp parallel
  {
    int id = omp_get_thread_num();
    int max = omp_get_num_threads();
    int start = id * N / max;
    int end = (id + 1) * N / max;
#pragma omp for
    {
      for (int i = start; i < end; i++)
      {
        for (int j = 0; j < N; j++)
        {
          for (int k = 0; k < N; k++)
          {
            ans += A[i][k] * B[k][j];
            operatorCount++;
          }
        }
      }
    }
  }

  mainED = omp_get_wtime();
  // 計測終了
  ed = omp_get_wtime();
  printf("Nの大きさ:%d\n", N);
  printf("実行回数%llu:\n", operatorCount);
  printf("主要処理実行時間: %lf sec\n", (double)(mainED - mainST));
  printf("その他: %lf sec\n", (double)((ed - st) - (mainED - mainST)));
  printf("全体実行時間: %lf sec\n", (double)(ed - st));
  return 0;
}

/*
実行コマンド
コンパイル：clang -Xpreprocessor -fopenmp -lomp myfile.cxx
  参考リンク：https://qiita.com/ktgw0316/items/23235dd2533f488be7da
実行時：env OMP_NUM_THREADS=2 ./a.out
*/

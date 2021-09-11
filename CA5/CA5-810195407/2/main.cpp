#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "ipp.h"

#define VECTOR_SIZE 1048576
#define NUM_THREADS 8
using namespace std;

typedef struct
{
  long startIdx;
  long endIdx;
} data;

float *fArray1;
float *fArray2;

void testit(float *v)
{
  int i;
  int not_sorted = 0;

  for (i = 0; i < VECTOR_SIZE - 1; i++)
    if (v[i] > v[i + 1])
    {
      not_sorted = 1;
      break;
    }
  if (not_sorted)
    printf("Array NOT sorted.\n");
  else
    printf("Array sorted.\n");
}

int partition(float arr[], int start, int end)
{
  int i, j;
  float key, temp;
  key = arr[start];
  i = start + 1;
  j = end;
  while (1)
  {
    while (i < end && key >= arr[i])
      i++;
    while (key < arr[j])
      j--;
    if (i < j)
    {
      temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
    else
    {
      temp = arr[start];
      arr[start] = arr[j];
      arr[j] = temp;
      return (j);
    }
  }
}

void serialQuicksort(float arr[], int start, int end)
{
  int pivot;

  if (start < end)
  {
    pivot = partition(arr, start, end);
    serialQuicksort(arr, start, pivot - 1);
    serialQuicksort(arr, pivot + 1, end);
  }
}

void *parallelQuicksort(void *arg)
{
  data *in = (data *)arg;
  long start = in->startIdx;
  long end = in->endIdx;
  int pivot;

  if (start < end)
  {
    pthread_t th1, th2;
    data *i1 = (data *)malloc(sizeof(data));
    data *i2 = (data *)malloc(sizeof(data));

    pivot = partition(fArray2, start, end);
    i1->startIdx = start;
    i1->endIdx = pivot - 1;
    i2->startIdx = pivot + 1;
    i2->endIdx = end;

    if (i1->endIdx - i1->startIdx > 10000)
    {
      pthread_create(&th1, NULL, parallelQuicksort, i1);
    }
    else
    {
      serialQuicksort(fArray2, i1->startIdx, i1->endIdx);
    }

    if (i2->endIdx - i2->startIdx > 10000)
    {
      pthread_create(&th2, NULL, parallelQuicksort, i2);
    }
    else
    {
      serialQuicksort(fArray2, i2->startIdx, i2->endIdx);
    }

    if (i1->endIdx - i1->startIdx > 10000)
    {
      pthread_join(th1, NULL);
    }
    if (i2->endIdx - i2->startIdx > 10000)
    {
      pthread_join(th2, NULL);
    }
    free(i1);
    free(i2);
  }

  pthread_exit(0);
}

int main(int argc, char const *argv[])
{
  Ipp64u start, end;
  Ipp64u time1, time2;

  fArray1 = new float[VECTOR_SIZE];
  fArray2 = new float[VECTOR_SIZE];

  if (!fArray1 || !fArray2)
  {
    printf("Memory allocation error!!\n");
    return 1;
  }

  // Initialize vectors with random numbers
  for (long i = 0; i < VECTOR_SIZE; i++)
  {
    fArray1[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
    fArray2[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
  }

  // Serial implementation
  start = ippGetCpuClocks();
  serialQuicksort(fArray1, 0, VECTOR_SIZE - 1);
  end = ippGetCpuClocks();
  time1 = end - start;

  // Parallel implementation
  start = ippGetCpuClocks();
  pthread_t th;
  data *in = (data *)malloc(sizeof(data));
  in->startIdx = 0;
  in->endIdx = VECTOR_SIZE - 1;
  pthread_create(&th, NULL, parallelQuicksort, in);
  pthread_join(th, NULL);
  end = ippGetCpuClocks();
  time2 = end - start;

  free(in);

  printf("Hossein Soltanloo\n");
  printf("Student Number: 810195407\n\n");

  printf("Serial Run time = %d \n", (Ipp32s)time1);
  printf("Parallel Run time = %d \n", (Ipp32s)time2);
  printf("Parallel Speedup = %f\n", (float)(time1) / (float)time2);

  printf("Test if arrays are sorted:\n");
  printf("Serial:\n");
  testit(fArray1);
  printf("Parallel:\n");
  testit(fArray2);

  pthread_exit(0);
}

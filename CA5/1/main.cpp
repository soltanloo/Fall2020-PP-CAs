#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "ipp.h"

#define VECTOR_SIZE 1048576
#define NUM_THREADS 8

typedef struct
{
  float value;
  long idx;
} maxElement;

typedef struct
{
  long startIdx;
  long endIdx;
} inParam;

float *fArray;

void *thread_func(void *arg)
{
  inParam *input = (inParam *)arg;
  float localMax = fArray[0];
  long localMaxIdx = 0;

  for (long idx = input->startIdx; idx < input->endIdx; idx++)
  {
    localMax = localMax > fArray[idx] ? localMax : fArray[idx];
    localMaxIdx = localMax > fArray[idx] ? localMaxIdx : idx;
  }

  maxElement *result = (maxElement *)malloc(sizeof(maxElement));
  result->value = localMax;
  result->idx = localMaxIdx;
  pthread_exit(result);
}

int main(int argc, char const *argv[])
{
  Ipp64u start, end;
  Ipp64u time1, time2;

  fArray = new float[VECTOR_SIZE];

  if (!fArray)
  {
    printf("Memory allocation error!!\n");
    return 1;
  }

  // Initialize vectors with random numbers
  for (long i = 0; i < VECTOR_SIZE; i++)
    fArray[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));

  // Serial implementation
  start = ippGetCpuClocks();
  float max1 = fArray[0];
  long max1idx = 0;
  for (long i = 0; i < VECTOR_SIZE; i += 1)
  {
    if (fArray[i] > max1)
    {
      max1 = fArray[i];
      max1idx = i;
    }
  }
  end = ippGetCpuClocks();
  time1 = end - start;

  start = ippGetCpuClocks();
  long maxIdx;
  float maxVal = 0;
  pthread_t th[NUM_THREADS];
  inParam in[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++)
  {
    in[i].startIdx = i * VECTOR_SIZE / NUM_THREADS;
    in[i].endIdx = (i + 1) * VECTOR_SIZE / NUM_THREADS;
  }

  void *thread_result;
  maxElement *return_value;
  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_create(&th[i], NULL, thread_func, (void *)&in[i]);
  }
  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(th[i], &thread_result);
    return_value = (maxElement *)thread_result;
    if (return_value->value > maxVal)
    {
      maxVal = return_value->value;
      maxIdx = return_value->idx;
    }

    free(return_value);
  }

  end = ippGetCpuClocks();
  time2 = end - start;

  printf("Hossein Soltanloo\n");
  printf("Student Number: 810195407");
  printf("\nThe serial result is  Value = %f    Index = %ld\n", max1, max1idx);
  printf("The parallel result is  Value = %f    Index = %ld\n", maxVal, maxIdx);
  printf("Serial Run time = %d \n", (Ipp32s)time1);
  printf("Parallel Run time = %d \n", (Ipp32s)time2);
  printf("Speedup = %f\n\n", (float)(time1) / (float)time2);

  pthread_exit(0);
}

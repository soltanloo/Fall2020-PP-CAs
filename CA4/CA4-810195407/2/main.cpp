#include <iostream>
#include <omp.h>
#include "stdio.h"
#include "stdlib.h"
#include "ipp.h"
#include "x86intrin.h"

#define VECTOR_SIZE 1048576

using namespace std;

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

void sectionQuicksort(float arr[], int start, int end)
{
  int pivot;

  if (start < end)
  {
    pivot = partition(arr, start, end);
#pragma omp parallel sections
    {
#pragma omp section
      {
        sectionQuicksort(arr, start, pivot - 1);
      }
#pragma omp section
      {
        sectionQuicksort(arr, pivot + 1, end);
      }
    }
  }
}

void taskQuicksort(float arr[], int start, int end)
{
  int pivot;

  if (start < end)
  {
    pivot = partition(arr, start, end);

#pragma omp task default(none) firstprivate(arr, start, pivot)
    taskQuicksort(arr, start, pivot - 1);
#pragma omp task default(none) firstprivate(arr, end, pivot)
    taskQuicksort(arr, pivot + 1, end);
  }
}

int main()
{
  Ipp64u start, end;
  Ipp64u time1, time2, time3;

  float *fArray1;
  float *fArray2;
  float *fArray3;
  fArray1 = new float[VECTOR_SIZE];
  fArray2 = new float[VECTOR_SIZE];
  fArray3 = new float[VECTOR_SIZE];

  if (!fArray1 || !fArray2 || !fArray3)
  {
    printf("Memory allocation error!!\n");
    return 1;
  }

  // Initialize vectors with random numbers
  for (long i = 0; i < VECTOR_SIZE; i++)
  {
    fArray1[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
    fArray2[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
    fArray3[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
  }

  // Serial implementation
  start = ippGetCpuClocks();
  serialQuicksort(fArray1, 0, VECTOR_SIZE - 1);
  end = ippGetCpuClocks();
  time1 = end - start;

  // Parallel implementation using sections
  start = ippGetCpuClocks();
  sectionQuicksort(fArray2, 0, VECTOR_SIZE - 1);
  end = ippGetCpuClocks();
  time2 = end - start;

  // Parallel implementation using tasks
  start = ippGetCpuClocks();
#pragma omp parallel default(none) shared(fArray3)
  {
#pragma omp single nowait
    {
      taskQuicksort(fArray3, 0, VECTOR_SIZE - 1);
    }
  }
  end = ippGetCpuClocks();
  time3 = end - start;

  printf("Hossein Soltanloo\n");
  printf("Student Number: 810195407\n\n");

  printf("Serial Run time = %d \n", (Ipp32s)time1);
  printf("Parallel (section) Run time = %d \n", (Ipp32s)time2);
  printf("Parallel (task) Run time = %d \n", (Ipp32s)time3);
  printf("Parallel (section) Speedup = %f\n", (float)(time1) / (float)time2);
  printf("Parallel (task) Speedup = %f\n\n", (float)(time1) / (float)time3);

  printf("Test if arrays are sorted:\n");
  printf("Serial:\n");
  testit(fArray1);
  printf("Parallel (section):\n");
  testit(fArray2);
  printf("Parallel (task):\n");
  testit(fArray3);

  return 0;
}
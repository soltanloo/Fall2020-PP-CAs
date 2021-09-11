#include "stdio.h"
#include "stdlib.h"
#include "ipp.h"
#include "x86intrin.h"

#define VECTOR_SIZE 1048576

int main(void)
{
	Ipp64u start, end;
	Ipp64u time1, time2;

	float *fArray;
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
	long idx, maxIdx;
	float maxVal;
	float localMax;
	long localMaxIdx;

#pragma omp parallel default(shared) private(localMax, localMaxIdx) num_threads(4)
	{
		localMax = fArray[0];
		localMaxIdx = 0;
#pragma omp for
		for (idx = 0; idx < VECTOR_SIZE; idx++)
		{
			localMax = localMax > fArray[idx] ? localMax : fArray[idx];
			localMaxIdx = localMax > fArray[idx] ? localMaxIdx : idx;
		}
#pragma omp critical
		{
			if (localMax > maxVal)
			{
				maxVal = localMax;
				maxIdx = localMaxIdx;
			}
		}
	}

	end = ippGetCpuClocks();
	time2 = end - start;

	printf("Hossein Soltanloo\n");
	printf("Student Number: 810195407");
	printf("\nThe serial result is  Value = %f    Index = %d\n", max1, max1idx);
	printf("The parallel result is  Value = %f    Index = %d\n", maxVal, maxIdx);
	printf("Serial Run time = %d \n", (Ipp32s)time1);
	printf("Parallel Run time = %d \n", (Ipp32s)time2);
	printf("Speedup = %f\n\n", (float)(time1) / (float)time2);

	return 0;
}

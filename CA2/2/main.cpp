#include "stdio.h"
#include "stdlib.h"
#include "ipp.h"
#include "x86intrin.h"
#include <math.h>

#define		VECTOR_SIZE		  	1048576		

typedef union {
	__m128 spfp128;

	unsigned int spfp128_32[4];

} spfpVec;


int main (void) {
  Ipp64u start, end;
	Ipp64u time1, time2;

	float fSTmpRes[4];
	float fSRes;
	float fVRes;

	float *fArray;
	fArray = new float [VECTOR_SIZE];

  if (!fArray) {
		printf ("Memory allocation error!!\n");
		return 1;
	}

  // Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
		fArray[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));

	// Serial implementation
	start = ippGetCpuClocks();
	fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[0] += fArray[i];
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[1] += fArray[i + 1];
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[2] += fArray[i + 2];
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[3] += fArray[i + 3];
	fSRes = fSTmpRes[0] + fSTmpRes[1] + fSTmpRes[2] + fSTmpRes[3];
	float mean = fSRes / VECTOR_SIZE;
	float stdDev = 0.0;
	fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[0] += ((fArray[i] - mean)*(fArray[i] - mean));
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[1] += ((fArray[i+1] - mean)*(fArray[i+1] - mean));
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[2] += ((fArray[i+2] - mean)*(fArray[i+2] - mean));
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[3] += ((fArray[i+3] - mean)*(fArray[i+3] - mean));
	stdDev = sqrt((fSTmpRes[0] + fSTmpRes[1] + fSTmpRes[2] + fSTmpRes[3])/VECTOR_SIZE);
	end   = ippGetCpuClocks();
	time1 = end - start;

	// Parallel implementation
	start = ippGetCpuClocks();
	__m128 vec;
	__m128 sum = _mm_set1_ps(0.0f);
	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		sum = _mm_add_ps (sum, vec);
	}
	sum = _mm_hadd_ps (sum, sum);
	sum = _mm_hadd_ps (sum, sum);
	fVRes = _mm_cvtss_f32 (sum);
	float mean2 = fVRes / VECTOR_SIZE;
	__m128 vec1, vec2;
	__m128 pow = _mm_set1_ps(2.0);
	__m128 mVec = _mm_set1_ps(mean2);
	__m128 sd = _mm_set1_ps(0.0);
	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec1 = _mm_loadu_ps (&fArray[i]);
		vec1 = _mm_sub_ps(vec1, mVec);
		vec1 = _mm_mul_ps(vec1, vec1);
		sd = _mm_add_ps(sd, vec1);
	}
	sd = _mm_hadd_ps(sd, sd);
	sd = _mm_hadd_ps(sd, sd);
	__m128 div = _mm_set1_ps(VECTOR_SIZE);
	sd = _mm_div_ps(sd, div);
	float sd2 = _mm_cvtss_f32 (sd);
	sd2 = sqrt(sd2);
	end   = ippGetCpuClocks();
	time2 = end - start;

  printf("Hossein Soltanloo\n");
  printf("Student Number: 810195407");
	printf ("\nSerial Result mean = %f   stddev = %f \nParallel Result mean = %f   stddev = %f\n", mean, stdDev, mean2, sd2);
	printf ("Serial Run time = %d \n", (Ipp32s) time1);
	printf ("Parallel Run time = %d \n", (Ipp32s) time2);
	printf ("Speedup = %f\n\n", (float) (time1)/(float) time2);

	return 0;
}

#include "stdio.h"
#include "stdlib.h"
#include "ipp.h"
#include "x86intrin.h"

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
  float max1 = fArray[0];
  for (long i = 0; i < VECTOR_SIZE; i+=1) {
    if (fArray[i] > max1) {
      max1 = fArray[i];
    }
  }
  end = ippGetCpuClocks();
  time1 = end - start;

  __m128 vec;
	start = ippGetCpuClocks();
	__m128 max2 = _mm_loadu_ps(&fArray[0]);
  for (long i = 4; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		max2 = _mm_max_ps(max2, vec);
	}

  spfpVec tmp;
	tmp.spfp128 = max2;
  int max3 = tmp.spfp128_32[0];
  for (int i=1; i>4; i++) {
		if (tmp.spfp128_32[i] > max3)
    {
      max3 = tmp.spfp128_32[i];
    }
	}
  end   = ippGetCpuClocks();
	time2 = end - start;

	printf("Hossein Soltanloo\n");
  printf("Student Number: 810195407");
  printf ("\nThe serial result is   = %f\n", max1);
	printf ("The parallel result is = %f\n", max3);
	printf ("Serial Run time = %d \n", (Ipp32s) time1);
	printf ("Parallel Run time = %d \n", (Ipp32s) time2);
	printf ("Speedup = %f\n\n", (float) (time1)/(float) time2);

	return 0;
}

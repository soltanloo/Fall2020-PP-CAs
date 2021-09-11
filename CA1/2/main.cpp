#include "stdio.h"
#include "x86intrin.h"

typedef union {
	__m128i 			int128;
	
	unsigned char		m128_u8[16];
	signed char			m128_i8[16];

	unsigned short		m128_u16[8];
	signed short		m128_i16[8];

	unsigned int m128_u32[4];
	signed int m128_i32[4];

	unsigned long m128_u64[2];
	signed long m128_i64[2];
} intVec;

typedef union {
	__m128 spfp128;

	unsigned int spfp128_32[4];

} spfpVec;

void print_u8 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%X, ", tmp.m128_u8[i]);
	}
	printf ("%X]\n\n", tmp.m128_u8[0]);
}

void print_i8 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%d, ", tmp.m128_i8[i]);
	}
	printf ("%d]\n\n", tmp.m128_i8[0]);
}

void print_u16 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_u16[i]);
	}
	printf ("%X]\n\n", tmp.m128_u16[0]);
}

void print_i16 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%d, ", tmp.m128_i16[i]);
	}
	printf ("%d]\n\n", tmp.m128_i16[0]);
}

void print_u32 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%X, ", tmp.m128_u32[i]);
	}
	printf ("%X]\n\n", tmp.m128_u32[0]);
}

void print_i32 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%d, ", tmp.m128_i32[i]);
	}
	printf ("%d]\n\n", tmp.m128_i32[0]);
}

void print_u64 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=1; i>0; i--) {
		printf ("%lX, ", tmp.m128_u64[i]);
	}
	printf ("%lX]\n\n", tmp.m128_u64[0]);
}

void print_i64 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=1; i>0; i--) {
		printf ("%ld, ", tmp.m128_i64[i]);
	}
	printf ("%ld]\n\n", tmp.m128_i64[0]);
}

void print_int_vector(__m128i a, unsigned char type) {
	// type :
	// 0x00: u8: print 16 unsigned byte
	// 0x01: i8: print 16 signed byte
	// 0x02: u16: print 8 unsigned word
	// 0x03: i16: print 8 signed word
	// 0x04: u32: print 4 unsigned double word
	// 0x05: i32: print 4 signed double word
	// 0x06: u64: print 2 unsigned quad word
	// 0x07: i64: print 2 signed quad word
	switch (type)
	{
	case 0x00:
		printf ("Unsigned byte (hex): ");
		print_u8 (a);
		break;
	case 0x01:
		printf ("Signed byte (decimal): ");
		print_i8 (a);
		break;
	case 0x02:
		printf ("Unsigned word (hex): ");
		print_u16 (a);
		break;
	case 0x03:
		printf ("Signed word (decimal): ");
		print_i16 (a);
		break;
	case 0x04:
		printf ("Unsigned double word (hex): ");
		print_u32 (a);
		break;
	case 0x05:
		printf ("Signed double word (decimal): ");
		print_i32 (a);
		break;
	case 0x06:
		printf ("Unsigned quad word (hex): ");
		print_u64 (a);
		break;
	case 0x07:
		printf ("Signed quad word (decimal): ");
		print_i64 (a);
		break;
	default:
		break;
	}
}

void print_spfp_vector(__m128 a) {
	printf("Single precision floating point (hex): ");
	spfpVec tmp;
	tmp.spfp128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%X, ", tmp.spfp128_32[i]);
	}
	printf ("%X]\n\n", tmp.spfp128_32[0]);
}

unsigned char intArray [16] = {	0X00, 0X11, 0X22, 0X33, 0X44, 0X55, 0X66, 0X77,
								0X88, 0X99, 0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0XFF};

unsigned int floatArray [4] = {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF};

int main(void)
{
	__m128i a;
	a = _mm_load_si128((const __m128i*)intArray);
	print_int_vector(a, 0x00);
	print_int_vector(a, 0x01);
	print_int_vector(a, 0x02);
	print_int_vector(a, 0x03);
	print_int_vector(a, 0x04);
	print_int_vector(a, 0x05);
	print_int_vector(a, 0x06);
	print_int_vector(a, 0x07);

	__m128 b;
	b = _mm_load_ps((const float*)floatArray);
	print_spfp_vector(b);
	return 0;
}

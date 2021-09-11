#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "ipp.h"
#include "x86intrin.h"
#include <cmath>

using namespace cv;
using namespace std;

uint8_t saturatedAdd(uint8_t a, uint8_t b)
{
	uint8_t c = a + b;
	if (c < a)
		c = 255;
	return c;
}

int main()
{
	Ipp64u start, end;
	Ipp64u time1, time2;

	// LOAD image
	cv::Mat outImg1 = cv::imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);
	cv::Mat outImg2 = cv::imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);

	cv::Mat img1 = cv::imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);
	cv::Mat img2 = cv::imread("CA03__Q2__Image__02.png", IMREAD_GRAYSCALE);

	unsigned int NCOLS1 = img1.cols;
	unsigned int NROWS1 = img1.rows;

	unsigned int NCOLS2 = img2.cols;
	unsigned int NROWS2 = img2.rows;

	// Create output images
	cv::Mat out_img1(img1.rows, img1.cols, CV_8U);
	cv::Mat out_img2(img1.rows, img1.cols, CV_8U);

	// Pointer to the image data (Matrix of pixels)
	unsigned char *in_image1;
	unsigned char *in_image2;
	unsigned char *out_image1;

	in_image1 = (unsigned char *)img1.data;
	in_image2 = (unsigned char *)img2.data;
	out_image1 = (unsigned char *)outImg1.data;

	// Serial implementation
	start = ippGetCpuClocks();
	for (int row = 0; row < NROWS2; row++)
		for (int col = 0; col < NCOLS2; col++)
		{
			unsigned char base, append;
			base = *(in_image1 + row * NCOLS1 + col);
			append = *(in_image2 + row * NCOLS2 + col);
			*(out_image1 + row * NCOLS1 + col) = saturatedAdd(base, append >> 1);
		}

	end = ippGetCpuClocks();
	time1 = end - start;

	__m128i *pSrc1;
	__m128i *pSrc2;
	__m128i *pRes;
	__m128i m1, m2, m3;

	pSrc1 = (__m128i *)img1.data;
	pSrc2 = (__m128i *)img2.data;
	pRes = (__m128i *)outImg2.data;

	// Parallel implementation
	start = ippGetCpuClocks();
	for (int i = 0; i < NROWS2; i++)
		for (int j = 0; j < NCOLS2 / 16; j++)
		{
			m1 = _mm_loadu_si128(pSrc1 + i * NCOLS1 / 16 + j);
			m2 = _mm_loadu_si128(pSrc2 + i * NCOLS2 / 16 + j);
			m3 = _mm_adds_epu8(m1, _mm_and_si128(_mm_set1_epi8(0xFF >> 1), _mm_srli_epi32(m2, 1)));
			_mm_storeu_si128(pRes + i * NCOLS1 / 16 + j, m3);
		}
	end = ippGetCpuClocks();
	time2 = end - start;

	//DISPLAY images
	cv::namedWindow("input1", cv::WINDOW_AUTOSIZE);
	cv::imshow("input1", img1);
	cv::namedWindow("input2", cv::WINDOW_AUTOSIZE);
	cv::imshow("input2", img2);
	cv::namedWindow("serial output", cv::WINDOW_AUTOSIZE);
	cv::imshow("serial output", outImg1);
	cv::imwrite("serial-output.png", outImg1);
	cv::namedWindow("parallel output", cv::WINDOW_AUTOSIZE);
	cv::imshow("parallel output", outImg2);
	cv::imwrite("parallel-output.png", outImg2);

	waitKey(0); // Wait for a keystroke in the window

	printf("Hossein Soltanloo - 810195407\n");
	printf("Serial Run time = %d \n", (Ipp32s)time1);
	printf("Parallel Run time = %d \n", (Ipp32s)time2);
	printf("Speedup = %4.2f\n", (float)(time1) / (float)time2);
	return 0;
}

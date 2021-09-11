#include <cpuid.h>
#include <stdio.h>
#include <stdint.h>

void cpuid(int CPUInfo[4], int InfoType) {
	__asm__ __volatile__(
		"cpuid":
	    "=a" (CPUInfo[0]),
		"=b" (CPUInfo[1]),
		"=c" (CPUInfo[2]),
		"=d" (CPUInfo[3]) :
		"a" (InfoType)
		);
}

int main(void) {
    printf("Hossein Soltanloo\n");
    printf("Student Number: 810195407\n\n");

    uint32_t brand[12];
    if (!__get_cpuid_max(0x80000004, NULL)) {
        fprintf(stderr, "Feature not implemented.");
        return 2;
    }
    __get_cpuid(0x80000002, brand+0x0, brand+0x1, brand+0x2, brand+0x3);
    __get_cpuid(0x80000003, brand+0x4, brand+0x5, brand+0x6, brand+0x7);
    __get_cpuid(0x80000004, brand+0x8, brand+0x9, brand+0xa, brand+0xb);
    printf("Brand and Model Name: %s\n\n", brand);

	int info[4];
	bool MMX   = false;
	bool SSE   = false;
	bool SSE2  = false;
	bool SSE3  = false;
	bool AES   = false;
	bool SSE41 = false;
	bool SSE42 = false;
	bool AVX   = false;
	bool AVX2  = false;
	bool SHA   = false;

	cpuid(info, 0x00000001);

	MMX   = (info[3] & ((int)1 << 23)) != 0;

	SSE   = (info[3] & ((int)1 << 25)) != 0;
	SSE2  = (info[3] & ((int)1 << 26)) != 0;
	SSE3  = (info[2] & ((int)1 << 0))  != 0;
	AES   = (info[2] & ((int)1 << 25)) != 0;
	SSE41 = (info[2] & ((int)1 << 19)) != 0;
	SSE42 = (info[2] & ((int)1 << 20)) != 0;

	AVX   = (info[2] & ((int)1 << 28)) != 0;

	cpuid(info, 0x80000000);
	if (info[0] >= 0x00000007){
		cpuid(info, 0x00000007);
		AVX2   = (info[1] & ((int)1 <<  5)) != 0;
		SHA    = (info[1] & ((int)1 << 29)) != 0;
	}
    printf("Supported SIMD Architectures:\n");
	printf("%s\n", MMX   ? "MMX:   Supported" : "MMX:   NOT Supported");
	printf("%s\n", SSE   ? "SSE:   Supported" : "SSE:   NOT Supported");
	printf("%s\n", SSE2  ? "SSE2:  Supported" : "SSE2:  NOT Supported");
	printf("%s\n", SSE3  ? "SSE3:  Supported" : "SSE3:  NOT Supported");
	printf("%s\n", SSE41 ? "SSE41: Supported" : "SSE41: NOT Supported");
	printf("%s\n", SSE42 ? "SSE42: Supported" : "SSE42: NOT Supported");
	printf("%s\n", AES   ? "AES:   Supported" : "AES:   NOT Supported");
	printf("%s\n", SHA   ? "SHA:   Supported" : "SHA:   NOT Supported");
	printf("%s\n", AVX   ? "AVX:   Supported" : "AVX:   NOT Supported");
	printf("%s\n", AVX2  ? "AVX2:  Supported" : "AVX2:  NOT Supported");

	return 0;
}

#include <complex.h>
#include <stdio.h>
#include <time.h>
#include "../inc/FFT.h"

int main(void)
{
	int N = 8;
	float realTestArray[N];
	float imagTestArray[N];
	float realOut[N];
	float imagOut[N];

	fft_config_t* conf = fft_init(N, NULL, NULL, NULL, NULL);

	for(int i = 0; i < N; i++)
	{
		realTestArray[i] = i;
		imagTestArray[i] = 0;
	}

	fft_execute(realTestArray, imagTestArray, realOut, imagOut, N);
	printf("FFT EXECUTE: \n");
	for(int i = 0; i < N; i++)
	{
		printf("Bin %i: %f + %fi\n", i, realOut[i], imagOut[i]);
	}

	fft_iterative(N, realTestArray, imagTestArray);
	printf("ITERATIVE FFT EXECUTE: \n");
	for(int i = 0; i < N; i++)
	{
		printf("Bin %i: %f + %fi\n", i, realTestArray[i], imagTestArray[i]);
	}

	return 0;
}

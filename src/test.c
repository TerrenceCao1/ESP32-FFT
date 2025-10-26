#include <complex.h>
#include <stdio.h>
#include "../inc/FFT.h"

int main(void)
{
	int N = 8;
	float realTestArray[N];
	float imagTestArray[N];
	float realOutput[N];
	float imagOutput[N];

	for(int i = 0; i < N; i++)
	{
		realTestArray[i] = 1.5 * i;
		imagTestArray[i] = -1.4 * i;
	}

	fft_execute(realTestArray, imagTestArray, realOutput, imagOutput, 8);

	for(int i = 0; i < N; i++)
	{
		printf("Bin %i: %.3f + %.3fi\n", i, realOutput[i], imagOutput[i]);
	}

	return 0;
}

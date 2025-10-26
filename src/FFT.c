#include "../inc/FFT.h"

fft_config_t* fft_init(int size, float* realInputBuff, float* imagInputBuff, float* realOutputBuff, float* imagOutputBuff)
{
	fft_config_t* conf = (fft_config_t*)malloc(sizeof(fft_config_t));

	//CHECK IF SIZE IS A POWER OF 2!
	if((size & (size-1)) != 0) //if size is a power of two it'll be like 0000001000 and size-1 would be 0000000111
	{
		free(conf);
		printf("Yo input a power of 2 size!\n");
		return NULL;
	}
	
	conf->size = size;

	//allocate the fft input buffers
	if(realInputBuff != NULL)
	{
		conf->realInput = realInputBuff;
	}
	else
	{
		conf->realInput = (float*)malloc(conf->size * sizeof(float));
	}

	if(imagInputBuff != NULL)
	{
		conf->imagInput = imagInputBuff;
	}
	else
	{
		conf->imagInput = (float*)malloc(conf->size * sizeof(float));
	}

	//allocate fft output buffers
	if(realOutputBuff != NULL)
	{
		conf->realOutput = realOutputBuff;
	}
	else
	{
		conf->realOutput = (float*)malloc(conf->size * sizeof(float) * 2); //for a complex fft, sizeof(output) == sizeof(input).
	}
	
	if(imagOutputBuff != NULL)
	{
		conf->imagOutput = realOutputBuff;
	}
	else
	{
		conf->imagOutput = (float*)malloc(conf->size * sizeof(float) * 2); //for a complex fft, sizeof(output) == sizeof(input).
	}

	//allocate and compute twiddle values
	conf->twiddleFactors = (float*)malloc(conf->size * 2 * sizeof(float)); /*1 float for real, one for imaginary per bin*/

	for(int k = 0; k < conf->size; k++)
	{
		//real
		conf->twiddleFactors[2 * k] = cosf(2 * PI * k / conf->size);
		//imaginary
		conf->twiddleFactors[2 * k + 1] = sinf(-2 * PI * k / conf->size);
	}

	return conf;
}

void fft_free(fft_config_t* fft)
{
	free(fft->realInput);
	free(fft->imagInput);
	free(fft->realOutput);
	free(fft->imagOutput);
	free(fft->twiddleFactors);
	free(fft);
}

void fft_execute(float* realInput, float* imagInput, float* realOutput, float* imagOutput, int N)
{
	//there's already a power of two check in fft_init so we don't gotta do it here.
	if(N == 1)
	{
		realOutput[0] = realInput[0];
		imagOutput[0] = imagInput[0];
		return;
	}

	int half = N / 2;

	//creating the input arrays for next recursion iteration
	float* realInEven = malloc(half * sizeof(float));
	float* realInOdd = malloc(half * sizeof(float));
	float* imagInEven = malloc(half * sizeof(float));
	float* imagInOdd = malloc(half * sizeof(float));

	for(int i = 0; i < half; i++)
	{
		realInEven[i] = realInput[2 * i];
		realInOdd[i]  = realInput[2 * i + 1];
		imagInEven[i] = imagInput[2 * i];
		imagInOdd[i]  = imagInput[2 * i + 1];
	}

	//creating ouitput arrays for next recursion iteration
	float* realOutEven = malloc(half * sizeof(float));
	float* realOutOdd = malloc(half * sizeof(float));
	float* imagOutEven = malloc(half * sizeof(float));
	float* imagOutOdd = malloc(half * sizeof(float));

	//goin next recursion
	fft_execute(realInEven, imagInEven, realOutEven, imagOutEven, half);
	fft_execute(realInEven, imagInEven, realOutEven, imagOutEven, half);

	for(int k = 0; k < half; k++)
	{
		float theta = -2.0 * PI * k / N;
		float twiddleFactorReal = cosf(theta);
		float twiddleFactorImag = sinf(theta);

		float oddTwiddledReal = twiddleFactorReal * realInOdd[k] - twiddleFactorImag * imagInOdd[k];
		float oddTwiddledImag = twiddleFactorReal * imagInOdd[k] + twiddleFactorImag * realInOdd[k];



	}

}

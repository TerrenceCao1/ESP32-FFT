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
	conf->realTwiddleFactors = (float*)malloc(conf->size * sizeof(float));
	conf->imagTwiddleFactors = (float*)malloc(conf->size * sizeof(float));

	for(int k = 0; k < conf->size; k++)
	{
		//real
		conf->realTwiddleFactors[k] = cosf(2 * PI * k / conf->size);
		//imaginary
		conf->imagTwiddleFactors[k] = sinf(-2 * PI * k / conf->size);
	}

	return conf;
}

void fft_free(fft_config_t* fft)
{
	free(fft->realInput);
	free(fft->imagInput);
	free(fft->realOutput);
	free(fft->imagOutput);
	free(fft->realTwiddleFactors);
	free(fft->imagTwiddleFactors);
	free(fft);
}

//TODO: OPTIMIZATIONS:
//	bit-reversal reordering
//	Precalculate Twiddles
//	radix-4/8 maybe?
//	Precoding Trivial Recursion Cases
void fft_execute(int N, float* realInput, float* imagInput, float* realOutput, float* imagOutput, float* realTwiddles, float* imagTwiddles)
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
	float* realTwidEven = malloc(half * sizeof(float));
	float* imagTwidEven = malloc(half * sizeof(float));
	float* realTwidOdd = malloc(half * sizeof(float));
	float* imagTwidOdd = malloc(half * sizeof(float));

	for(int i = 0; i < half; i++)
	{
		realInEven[i] = realInput[2 * i];
		realInOdd[i]  = realInput[2 * i + 1];
		imagInEven[i] = imagInput[2 * i];
		imagInOdd[i]  = imagInput[2 * i + 1];
		realTwidEven[i] = realTwiddles[2 * i];
		imagTwidEven[i] = imagTwiddles[2 * i];
		realTwidOdd[i] = realTwiddles[2 * i + 1];
		imagTwidOdd[i] = imagTwiddles[2 * i + 1];
	}

	//creating output arrays for next recursion iteration
	float* realOutEven = malloc(half * sizeof(float));
	float* realOutOdd = malloc(half * sizeof(float));
	float* imagOutEven = malloc(half * sizeof(float));
	float* imagOutOdd = malloc(half * sizeof(float));

	//goin next recursion
	fft_execute(half, realInEven, imagInEven, realOutEven, imagOutEven, realTwidEven, imagTwidEven);
	fft_execute(half, realInOdd, imagInOdd, realOutOdd, imagOutOdd, realTwidOdd, imagTwidOdd);

	for(int k = 0; k < half; k++)
	{
		float realTwiddledOdd = realTwiddles[k] * realOutOdd[k] - imagTwiddles[k] * imagOutOdd[k];
		float imagTwiddledOdd = realTwiddles[k] * imagOutOdd[k] + imagTwiddles[k] * realOutOdd[k];

		float outReal_k     = realOutEven[k] + realTwiddledOdd;
		float outImag_k     = imagOutEven[k] + imagTwiddledOdd;
		float outReal_kHalf = realOutEven[k] - realTwiddledOdd;
		float outImag_kHalf = imagOutEven[k] - imagTwiddledOdd;

		realOutput[k]        = outReal_k;
		realOutput[k + half] = outReal_kHalf;
		imagOutput[k]        = outImag_k;
		imagOutput[k + half] = outImag_kHalf;
	}

	//Free them bruh
	free(realInEven); free(realInOdd);
	free(imagInEven); free(imagInOdd);
	free(realOutEven); free(realOutOdd);
	free(imagOutEven); free(imagOutOdd);
}

void real_fft_execute(fft_config_t* fft)
{
	float* zeroArr = calloc(fft->size, sizeof(float)); 
	if(!zeroArr)
	{
		free(zeroArr);
		return;
	}
	fft_execute(fft->size, fft->realInput, zeroArr, fft->realOutput, fft->imagOutput, fft->realTwiddleFactors, fft->imagTwiddleFactors);
	free(zeroArr);
}

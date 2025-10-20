#include "../inc/FFT.h"

fft_config_t* fft_init(int size, float* inputBuff, float* outputBuff)
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

	//allocate the fft input buffer
	if(inputBuff != NULL)
	{
		conf->input = inputBuff;
	}
	else
	{
		conf->input = (float*)malloc(conf->size * sizeof(float));
	}

	//allocate fft output buffer
	if(outputBuff != NULL)
	{
		conf->output = outputBuff;
	}
	else
	{
		conf->output = (float*)malloc(conf->size * sizeof(float));
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
	free(fft->input);
	free(fft->output);
	free(fft->twiddleFactors);
	free(fft);
}


void dft_execute(fft_config_t* fft)
{
	for(int bin = 0; bin < fft->size / 2; bin++)
	{
		float realComp = 0;
		float imagComp = 0;

		for(int i = 0; i < fft->size; i++)
		{
			realComp += fft->input[i] * (cosf(2 * PI * bin * i / fft->size));
			imagComp += fft->input[i] * (sinf(2 * PI * bin * i / fft->size));
		}

		fft->output[2 * bin] = realComp;
		fft->output[2 * bin + 1] = imagComp;
	}
}

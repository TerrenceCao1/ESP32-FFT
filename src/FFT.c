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

	return conf;
}

void fft_free(fft_config_t* fft)
{
	free(fft->input);
	free(fft->output);
	free(fft);
}

#include <stdio.h>
#include "../inc/FFT.h"
int main(void)
{
	int N = 8;

	//initiate the FFT Config
	fft_config_t* conf = fft_init(N, NULL, NULL);

	//populate the input arrays
	for(int i = 0; i < N; i++)
	{
		conf->realInput[i] = i;
		conf->imagInput[i] = -i;
	}

	//complex FFT (iterative, inplace)
	complex_fft_execute(conf);
	for(int i = 0; i < conf->size; i++)
	{
		//OUTPUTS GET PUT INTO THE INPUT ARRAYS!
		printf("Bin %i: %f + %fi\n", i, conf->realInput[i], conf->imagInput[i]);
	}

	fft_free(conf);
	return 0;
}

#ifndef FFT_H
#define FFT_H

//custom library for a real forward FFT 
#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.1415926538979323

typedef struct fft_config_t
{
	int size; //must be power of 2
	float* input;
	float* output;
}fft_config_t;

/* 
 * @brief	Initializes fft_config_t
 *
 *			Configures size, possible input and output buffer
 *
 * @param	size - size of FFT, how many samples taken in a time-domain signal
 *
 * @param	inputBuff - pointer to float array that contains samples to be FFT'd
 *
 * @param	outputBuff - pointer to float array that contains FFT Output. Each frequency bin
 *			will be 2 wide, with ki being the real part, and ki + 1 being the imaginary part for
 *			each buffer i.
 * 
 * @return	pointer to initiated fft_config_t fft structure. 
 *
 */
fft_config_t* fft_init(int size, float* inputBuff, float* outputBuff);

void fft_execute(fft_config_t*); 

void fft_free(fft_config_t*);


#endif //FFT_H

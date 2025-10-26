#ifndef FFT_H
#define FFT_H

#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.1415926538979323

typedef struct fft_config_t
{
	int size; //must be power of 2
	float* realInput;
	float* imagInput;
	float* realOutput;
	float* imagOutput;
	float* twiddleFactors;
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
fft_config_t* fft_init(int size, float* realInputBuff, float* imagInputBuff, float* realOutputBuff, float* imagOutputBuff);

/*
 * @brief	Radix-2 Cooley-Tukey Fast Fourier Transform
 *
 *			Computes complex FFT
 * 
 * @param	fft - pointer to fft_config_t to be FFT'd
 *
 * @return	void (but the fft->output will be filled)
 *
 */
void fft_execute(float* realInput, float* imagInput, float* realOutput, float* imagOutput, int N);

/*
 * @brief	REAL Radix-2 Cooley-Tukey Fast Fourier Transform
 *
 *			Computes real FFT
 * 
 * @param	fft - pointer to fft_config_t to be FFT'd
 *
 * @return	void (but the fft->output will be filled)
 *
 */
void real_fft_execute(fft_config_t* fft);

/*
 * @brief	Frees all memory allocated by fft_init
 *
 *			Frees the input buffer, output buffer, twiddle buffer
 *
 * @param	fft - pointer to fft_config_t to be destroyed
 *
 * @return	void
 * 
 */
void fft_free(fft_config_t* fft);

#endif //FFT_H

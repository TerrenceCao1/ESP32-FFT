#include "../inc/FFT.h"

//HELPERS!
static void reorder(float* inputArr, int size);

static unsigned int bit_reverse(unsigned int x, unsigned int bits);


fft_config_t* fft_init(int size, float* realInputBuff, float* imagInputBuff)
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
		conf->realInput = (float*)calloc(conf->size, sizeof(float));
	}

	if(imagInputBuff != NULL)
	{
		conf->imagInput = imagInputBuff;
	}
	else
	{
		conf->imagInput = (float*)calloc(conf->size, sizeof(float));
	}

	return conf;
}

void fft_free(fft_config_t* fft)
{
	free(fft->realInput);
	free(fft->imagInput);
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

	//creating output arrays for next recursion iteration
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

		float realTwiddledOdd = twiddleFactorReal * realInOdd[k] - twiddleFactorImag * imagInOdd[k];
		float imagTwiddledOdd = twiddleFactorReal * imagInOdd[k] + twiddleFactorImag * realInOdd[k];

		float realEven = realInEven[k];
		float imagEven = imagInEven[k];

		float outReal_k     = realEven + realTwiddledOdd;
		float outImag_k     = imagEven + imagTwiddledOdd;
		float outReal_kHalf = realEven - realTwiddledOdd;
		float outImag_kHalf = imagEven - imagTwiddledOdd;

		realOutput[k]        = outReal_k;
		realOutput[k + half] = outReal_kHalf;
		imagOutput[k]        = outImag_k;
		imagOutput[k + half] = outImag_kHalf;
	}

}

void real_fft_execute(fft_config_t* fft)
{
	memset(fft->imagInput, 0, fft->size * sizeof(float));
	fft_iterative(fft->size, fft->realInput, fft->imagInput);
}

void fft_iterative(int size, float* realInput, float* imagInput)
{
	reorder(realInput, size);
	reorder(imagInput, size);

	for(int step = 1; step <= (int)log2(size); step++)
	{
		int m = 1 << step;
		int half_m = m >> 1;

		//twiddles (w_m_r and w_m_i are real and imaginary twiddles AT THIS STAGE)
		float theta = -2.0f * PI / m;
		float w_m_r = cosf(theta);
		float w_m_i = sinf(theta);

		for(int k = 0; k < size; k += m)
		{
			//creating complex number: 1 + 0i as a base multiplier for w_m to get next twiddle
			float w_r = 1.0f;
			float w_i = 0.0f;

			for(int j = 0; j < half_m; j++)
			{
				int i_even = k + j;
				int i_odd = k + j + half_m;

				float t_real = w_r * realInput[i_odd] - w_i * imagInput[i_odd];
				float t_imag = w_r * imagInput[i_odd] + w_i * realInput[i_odd];

				float u_real = realInput[i_even];
				float u_imag = imagInput[i_even];

				realInput[i_even] = u_real + t_real;
				imagInput[i_even] = u_imag + t_imag;
				realInput[i_odd] = u_real - t_real;
				imagInput[i_odd] = u_imag - t_imag;

				float tmp_real = w_r * w_m_r - w_i * w_m_i;
				float tmp_imag = w_r * w_m_i + w_i * w_m_r;
				w_r = tmp_real;
				w_i = tmp_imag;
			}
		}
	}
}

void reorder(float* inputArr, int size) {
	if((size & (size-1)) != 0)
	{
		return;
	}

	float* copyArr = malloc(sizeof(float) * size);
	int bits = (int)log2(size);
	for(int i = 0; i < size; i++)
	{
		copyArr[i] = inputArr[bit_reverse(i, bits)];
	}

	memcpy(inputArr, copyArr, sizeof(float) * size);
	free(copyArr);
}

unsigned int bit_reverse(unsigned int x, unsigned int bits)
{
	unsigned int reversed = 0;
	for(int i = 0; i < bits; i++)
	{
		reversed = (reversed << 1) | (x & 1);
		x >>= 1;
	}
	return reversed;
}

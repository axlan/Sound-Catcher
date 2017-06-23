#ifndef FIXFFT_H
#define FIXFFT_H

#include <stdint.h>

/*
 fix_fft() - perform forward/inverse fast Fourier transform.
 fr[n],fi[n] are real and imaginary arrays, both INPUT AND
 RESULT (in-place FFT), with 0 <= n < 2**m; set inverse to
 0 for forward transform (FFT), or 1 for iFFT.
*/
int fix_fft(int8_t  fr[], int8_t  fi[], int m, int inverse);







#endif
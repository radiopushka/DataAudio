//PSK demodulation
//Evan Nikitin 2025
//PSK demodulation with no PLL
//No PID controller or feedback like in Costas or Tan Delay loops
//uses a continues angle counter 100*2pi to -100*2pi
//Fourie based demodulation
//angle is passed through a low pass filter
//sampling freq set by sampling window, PSK must be modulated at half this frequency
//Will support infitinityPSK as long as the running phase average is zero
//Vostok Telecom Russia
//example: carrier is 17.5khz
//phase modulation is 250hz sine
//window size is set to sample at 2khz, 24 samples per window at sound card rate of 48khz
//250 hz is well below nyquist 1 khz
#ifndef PSK_H
#define PSK_H
#include "../FFT/FFT.h"

struct PSK{
    FFT fft;

    int window_size;

};

#endif

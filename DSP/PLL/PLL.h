#ifndef PLL_H
#define PLL_H
#include "../FFT/FFT.h"
#include <math.h>

#define M_2PI 2 * M_PI
#define M_2PI3 (M_PI/2)*3

//Evan Nikitin 2025
//FFT sampling filter and simple LPF
//For BPSK demodulation

struct PLL{
    double pll_shifter;
    double pll_phi;
    double pll_phase;

    double costas_i;
    double costas_q;


    int pre_reset;//because FFT reset every FFT cache window
    int reset_value;

    int window_size;
    float* loop_buffer;

};
struct PLL_Component{
    double pll_shifter;
    double pll_phi;
};

typedef struct PLL* PLL;
typedef struct PLL_Component* PLL_Component;

//create a synchhornized harmonic oscillator as a product of the pll frequency
//scaling is the multiplier
PLL_Component create_PLL_Component(PLL p,float scaling);
void free_PLL_Component(PLL_Component p);

//the PLL generate must be called with tune window to keep them in sync
//returns the average value over this window
//it overwrites the output_buffer with the generated component, does not have to be the same output buffer used in PLL_tune_window
//the output is from -pi to pi
double PLL_generate_component(PLL p,PLL_Component pllc,float* output_buffer);

PLL create_PLL(float frequency_hz,int oversampling,int sample_rate,int window_size);

void free_PLL(PLL p);


//uses a running average to detect phase changes in the carrier
//filter is the filtering coefficient,convergance speed
double PLL_tune_window(PLL p,float* input_buffer,float filter);

//reset the PLL
void reset_PLL(PLL p);


#endif

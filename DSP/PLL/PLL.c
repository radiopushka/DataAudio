#include "PLL.h"
#include <stdlib.h>
#include <string.h>
//Evan Nikitin 2025
//Costas Loop With FFT sampling filter and simple LPF
//For BPSK demodulation

PLL create_PLL(float frequency_hz,int oversampling,int sample_rate,int window_size){
    PLL p = malloc(sizeof(struct PLL));
    p->window_size = window_size;
    p->pll_shifter = (frequency_hz/((float)sample_rate))*(2*M_PI);
    p->pll_phase = 0;
    p->pll_phi = 0;

    p->costas_i = 0;
    p->costas_q = 0;


    return p;

}

void free_PLL(PLL p){
    free(p);
}

void reset_PLL(PLL p){
    p->pll_phi = 0;
    p->pll_phase = 0;
}

PLL_Component create_PLL_Component(PLL p,float scaling){
    PLL_Component pllc = malloc(sizeof(struct PLL_Component));
    pllc->pll_shifter = p->pll_shifter * scaling;
    pllc->pll_phi = p->pll_phi;
    return pllc;
}
void free_PLL_Component(PLL_Component p){
    free(p);
}

double PLL_generate_component(PLL p,PLL_Component pllc,float* output_buffer){
    float* restrict end =output_buffer + p->window_size;

    double avrg = 0;
    for(;output_buffer<end;output_buffer++){
        pllc->pll_phi += pllc->pll_shifter;
        if(pllc->pll_phi > M_2PI){
            pllc->pll_phi -= M_2PI;
        }
       *output_buffer = (float) sin(pllc->pll_phi + p->pll_phase) * M_PI;
        avrg += *output_buffer;

    }
    return avrg/p->window_size;

}
double lpf(double prev,double input,double coeff){
    return prev*(1-coeff) + input*coeff;

}
double PLL_tune_window(PLL p, float *input_buffer, float filter){


    float* ien = input_buffer + p->window_size;
    double avg_i = 0;
    double avg_q = 0;
    for(float* itr = input_buffer;itr<ien;itr++){
        p->pll_phi += p->pll_shifter;
        if(p->pll_phi > M_2PI){
            p->pll_phi -= M_2PI;
        }
        double i = sin(p->pll_phi +p->pll_phase) * ((double)*itr);
        double q = cos(p->pll_phi +p->pll_phase) * ((double)*itr);
        avg_i += i;
        avg_q += q;

    }
    avg_i /= (double)p->window_size;
    avg_q /=(double)p->window_size;

    double mlt = 1/(filter/2);

    double ni = atan2(-avg_i,avg_q);



    double theta_lpf = ni;


    p->pll_phase = theta_lpf + lpf(p->pll_phase,theta_lpf,mlt);


        while(p->pll_phase>M_2PI){
            p->pll_phase -= M_2PI;
        }
        while(p->pll_phase<-M_2PI){
            p->pll_phase += M_2PI;
        }


    return ni;//returns from 0 to pi where modulation is from pi/2 to 3pi/2 or from 0 to pi
}

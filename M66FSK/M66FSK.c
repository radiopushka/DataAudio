#include "M66FSK.h"
#include <stdlib.h>
#include <string.h>

//carriers switch at 8hz, lpf can be half of sf
M66FSK_M create_66fsk_mod(double start_freq, int sample_rate, double sf){
    M66FSK_M mod = malloc(sizeof(struct M66FSK_m));
    memset(mod,0,sizeof(struct M66FSK_m));
    mod->sf = sf;
    mod->sample_size = sample_rate/8.0;

    double lpf_factor = (sample_rate/2.0)/sf;
    mod->alpha = 1.0/lpf_factor;

    double strt_f = start_freq;
    for(int i = 0;i<33;i++){
           mod->gen[i] = create_Synth(strt_f,sample_rate*4);
           strt_f += sf;
           strt_f += sf;
    }
    return mod;
}
M66FSK_D create_66fsk_demod(double start_freq, int sample_rate, double sf){
    M66FSK_D demod = malloc(sizeof(struct M66FSK_d));
    memset(demod,0,sizeof(struct M66FSK_d));
    demod->sample_size = sample_rate/8.0;
    demod->demod_list = malloc(demod->sample_size*sizeof(float));
    demod->samp_mult = 3;
    demod->samp_freq = demod->sample_size/demod->samp_mult;

    const int freq = 66;
    float freqs[freq];
    double strt_f = start_freq;
    for(int i = 0;i<freq;i++){
        freqs[i] = strt_f;
        strt_f += sf;
    }
    demod->demod = create_FFT(freqs,freq,10,sample_rate,sample_rate);
    return demod;
}
int fsk_get_buffer_size_m(M66FSK_M m){
    return m->sample_size;
}
int fsk_get_buffer_size_d(M66FSK_D d){
    return d->sample_size;
}

void free_66fsk_mod_m(M66FSK_M m){
    for(int i = 0;i<33;i++){
        free_Synth(m->gen[i]);
    }
    free(m);
}
void free_66fsk_mod_d(M66FSK_D d){
    free_FFT(d->demod);
    free(d->demod_list);
    free(d);
}
unsigned int pop_fsk_uint(M66FSK_D d){

    unsigned int ret = d->data_stack[0];
    for(int i = 0;i<3;i++){
        d->data_stack[i] = d->data_stack[i+1];
    }
    return ret;
}

//protocol

void _modulate_lpfs(M66FSK_M m,unsigned int data,int clock){

    for(int i = 0;i<32;i++){
        int val =  (data >> (31 - i))&1;
        double freq = (val ==  0)? 0.0 : m->sf;
        m->lpf[i] = m->lpf[i]*(1-m->alpha) + (m->alpha)*freq;
    }
    double cfreq = (clock ==  0)? 0.0 : m->sf;
    m->lpf[32] = m->lpf[32]*(1-m->alpha) + (m->alpha)*cfreq;
}

double _create_sample(M66FSK_M m){

    double sample = 0;
    for(int i = 0;i<33;i++){
        double v1 = next_Sval_FM(m->gen[i], m->lpf[i]);
        double v2 = next_Sval_FM(m->gen[i], m->lpf[i]);
        double v3 = next_Sval_FM(m->gen[i], m->lpf[i]);
        double v4 = next_Sval_FM(m->gen[i], m->lpf[i]);
        sample += (v1+v2+v3+v4)*248.0;
    }
    return sample;
}

void fsk_put_uint(M66FSK_M m,unsigned int data,short* buffer){

    int clock = ~m->clock;
    m->clock = clock;
    for(int i = 0;i<m->sample_size;i++){

        _modulate_lpfs(m, data, clock);
        double samp = _create_sample(m);
        *buffer = samp;
        buffer++;
    }

}

void data_stack_push(M66FSK_D d,unsigned int data){
    for(int i = 3;i>=0;i--){
        d->data_stack[i+1] = d->data_stack[i];
    }
    d->data_stack[0] = data;
}

unsigned int fsk_get_uint(M66FSK_D d,short* buffer){
    float* restrict cnvp = d->demod_list;
    for(short* sp = buffer;sp<buffer+d->sample_size;sp++){

        *cnvp = (float) *sp;
        cnvp++;
    }

    unsigned int output = 0;

    int buff_smp = d->samp_freq;

    cnvp = d->demod_list;
    for(int i = 0;i<d->samp_mult;i++){
        double* restrict amps = calculate_FFT(d->demod,cnvp,buff_smp);

        //get clock first
        double c_zero = amps[64];
        double c_one = amps[65];
        int cur_clock = (c_zero > c_one)? 0:~0;

        if(cur_clock != d->clock ){

            unsigned int val = 0;
            //convert to binary
            for(int i = 0;i<64;i = i + 2){
                double zero = d->r_avg[i]/d->samp_count;
                d->r_avg[i] = 0;
                double one = d->r_avg[i+1]/d->samp_count;
                d->r_avg[i+1] = 0;
                int shift = (zero > one)? 0:1;
                val = (val<<1)|shift;
            }
            data_stack_push(d,val);

            output++;
            d->clock = cur_clock;
            d->samp_count = 0;
        }

        for(double* restrict ins = d->r_avg;ins<d->r_avg + 64;ins++){
            *ins = *ins + *amps;
            amps++;
        }
        d->samp_count = d->samp_count + 1.0;



        cnvp += buff_smp;
    }

    return output;
}


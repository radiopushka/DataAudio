#include "CFSK.h"
#include <stdlib.h>
#include <string.h>

//Evan Nikitin 2025

CFSK_mod create_cfsk_mod(double start_freq, double sf, int sample_rate,int com_rate) {
    CFSK_mod mod = malloc(sizeof(struct CFSK_mod));

    mod->rate = sample_rate;
    mod->sf = sf;
    mod->crc_value = 250;

    mod->mod_sample_rate = sample_rate / com_rate;

    mod->mod_synth = create_Synth(start_freq, sample_rate*4);
    //smoothing factor per frequency transition
    float lpf_factor =(sample_rate/sf)/16.0;
    mod->amp_alpha = 1.0f/lpf_factor;
    mod->f_lp = 0;

    mod->multi_fmod = sf*0.04f;
    mod->multi_synth = create_Synth(com_rate*2, sample_rate*4);

    return mod;
}
CFSK_demod create_cfsk_demod(double start_freq, double sf, int sample_rate,int com_rate) {
    CFSK_demod mod = malloc(sizeof(struct CFSK_demod));

    mod->rate = sample_rate;

    mod->mod_sample_rate = sample_rate / (com_rate <<2);
    mod->crc_value = 250;

    const int m_freq = 64;
     float freqs[m_freq];
     for(int i = 0;i<m_freq;i++){
         freqs[i]=start_freq + (sf * i);
     }

    mod->freqs = m_freq;
    mod->demod_fft = create_FFT(freqs,m_freq,10,sample_rate,sample_rate);

    mod->shift_reg = 0;
    mod->framec = malloc(sizeof(float)*(mod->mod_sample_rate<<4));
    mod->sample_clock = 0;
    memset(mod->buffer,0,4);


    return mod;
}

void free_cfsk_mod(CFSK_mod* mod){

    free_Synth((*mod)->mod_synth);
    free_Synth((*mod)->multi_synth);
    free(*mod);
    *mod = NULL;
}
void free_cfsk_demod(CFSK_demod* mod){

    free_FFT((*mod)->demod_fft);
    free((*mod)->framec);
    free(*mod);
    *mod = NULL;
}

int get_demod_bsize(CFSK_demod mod){
    return mod->mod_sample_rate<<4;
}int get_mod_bsize(CFSK_mod mod){
    return mod->mod_sample_rate<<2;
}

int chr_extrct(unsigned char first,int samples){
    if(samples >2){
        return first;
    }else{
        return -1;
    }
}
int get_cfsk_data(CFSK_demod demod, short *buffer){
    float* restrict cnvp = demod->framec;
    int lc_sample_rate = demod->mod_sample_rate<<4;
    for(short* sp = buffer;sp<buffer+lc_sample_rate;sp++){
        *cnvp = (float) *sp;
        cnvp++;
    }
    int ret_val = -1;

    unsigned char* restrict buf = demod->buffer;
    int start_index = 0;
    for(int i = 0;i<16;i++){
        double* restrict amps = calculate_FFT(demod->demod_fft,demod->framec+start_index,demod->mod_sample_rate);

        start_index+=demod->mod_sample_rate;

        double mval = 0;
        int m_index = 0;
        for(int f = 0;f<demod->freqs;f++){
            double fval = amps[f];
            if(fval > mval){
                mval = fval;
                m_index = f;
            }

        }

        unsigned char prev = *buf;
        memmove(buf+1, buf, 3);
        *buf = m_index;
        demod->sample_clock++;

         //extract the actual 6 bit character
             int has_char = -1;
             if(demod->sample_clock >= 4){
                has_char = chr_extrct(prev,demod->sample_clock);
                 demod->sample_clock = 0;
                 memset(buf,0,4);
             }else if(prev != m_index && prev!=0){
                has_char = chr_extrct(prev,demod->sample_clock);
                demod->sample_clock = 0;
                memset(buf,0,4);
             }
             if(has_char != -1){
                 demod->shift_reg = (demod->shift_reg << 6) | has_char;
                 unsigned chk_crc = demod->shift_reg & 16777215;
                 if(chk_crc % demod->crc_value == 0){
                     ret_val = chk_crc/demod->crc_value;
                 }
             }



    }
    return ret_val;

}

void put_cfsk_data(CFSK_mod mod,unsigned short data, short* buffer){
    int crc_frame = data;
    crc_frame *= mod->crc_value;


    unsigned char dbuff[4];

    unsigned char* bpt = dbuff;
    unsigned char* bpt_e = dbuff+4;

    dbuff[3] = crc_frame & 63;
    dbuff[2] = (crc_frame >> 6) & 63;
    dbuff[1] = (crc_frame >> 12) & 63;
    dbuff[0] = (crc_frame >> 18) & 63;

    int chirp_c = 0;
    double phase = (*bpt) * mod->sf;

    double amp = 0;

    int rate_s = mod->mod_sample_rate <<2;



    for(short* p = buffer;p<buffer+rate_s;p++){
        if(chirp_c >= mod->mod_sample_rate){
            chirp_c = 0;
            bpt++;
            phase = (*bpt) * mod->sf;
            amp = 0;
        }

        double lcphase = mod->f_lp*(1-mod->amp_alpha) + (mod->amp_alpha)*phase;
        mod->f_lp = lcphase;
        double m1 = next_Sval(mod->multi_synth, 0)*mod->multi_fmod;
        double v1 = next_Sval_FM(mod->mod_synth, lcphase + m1);
        double m2 = next_Sval(mod->multi_synth, 0)*mod->multi_fmod;
        double v2 = next_Sval_FM(mod->mod_synth, lcphase + m2);
        double m3 = next_Sval(mod->multi_synth, 0)*mod->multi_fmod;
        double v3 = next_Sval_FM(mod->mod_synth, lcphase + m3);
        double m4 = next_Sval(mod->multi_synth, 0)*mod->multi_fmod;
        double v4 = next_Sval_FM(mod->mod_synth, lcphase + m4);

        *p = (short)((v1+v2+v3+v4)*8180.0f);


        chirp_c++;
    }
}


#ifndef CFSK_MOD
#define CFSK_MOD

#include "../DSP/DSP.h"

//chirp like spread spectrum FSK like modulation resistant to doppler effect and phase error
//maximum data rate of 120 bits per second but 8 chars per second due to CRC at 20Hz communication sampling frequency
//maximum approximate target - source moving speed while communicating:
//0,29 m/s in air or ~1km/h
//and 5.3 km/h in water
//290 km/s, 1 044 000 km/h in RF (assuming it is modulated as is from 0 Hz)
//this is 37 times faster than the speed of the ISS
//you would need to scale this at higher RF fequencies to get the same doppler resistance, if you multiply freq by 10 you multiply bandwidth by 10, spreading factor by 10
//since the is a mono frequency protocol the range is also good, not tested, but in air up to 300 meters with a 10 watt speaker.
//set sf to 44.0f for sound
//Vostok Telekom
//
//Evan Nikitin 2025


struct CFSK_demod{
    int crc_value;
    int rate;
    int mod_sample_rate;
    int freqs;
    float* framec;
    FFT demod_fft;

    unsigned char buffer[4];
    unsigned int sample_clock;

    unsigned int shift_reg;

};
struct CFSK_mod{
    int crc_value;
    int rate;
    int mod_sample_rate;
    double sf;
    double amp_alpha;
    double f_lp;



    Synth mod_synth;

    //about +-3% FM modulation to combat multipath fading a little
    double multi_fmod;
    Synth multi_synth;

};
typedef struct CFSK_mod* CFSK_mod;
typedef struct CFSK_demod* CFSK_demod;

CFSK_mod create_cfsk_mod(double start_freq,double sf, int sample_rate,int com_smpl_freq);
CFSK_demod create_cfsk_demod(double start_freq, double sf, int sample_rate,int com_smpl_freq);

int get_mod_bsize(CFSK_mod mod);
int get_demod_bsize(CFSK_demod mod);

int get_cfsk_data(CFSK_demod demod, short* buffer);
//data is a 16 bit integer, 2 bytes, 2 lower bytes
void put_cfsk_data(CFSK_mod mod,unsigned short data, short* buffer);

void free_cfsk_mod(CFSK_mod* mod);
void free_cfsk_demod(CFSK_demod* demod);


#endif

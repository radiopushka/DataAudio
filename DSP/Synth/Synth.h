#ifndef SYNTH_H
#define SYNTH_H
#include <math.h>
#define M_2PI 2 * M_PI

struct Synth{
    double shifter;
    double phi;
    double prev;
    double cur;
    double rate;

};
typedef struct Synth* Synth;

//create sine wave synthesizer
//frequency in Hz and sample rate
Synth create_Synth(float freq,int sample_rate);
//free synthesizer
void free_Synth(Synth s);
//return the next Synth value
//PSK modulation
double next_Sval(Synth s,double phase);
//FM modulation fshift is in Hz
double next_Sval_FM(Synth s,double fshift);

//return 1 if the value previously taken from next_Sval is after or is a cross of zero
int cross_Synth(Synth s);

//rewind the synthesizer by 1 sample
void rewind_Synth(Synth s);
void reset_Synth(Synth s);

#endif

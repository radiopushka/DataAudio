#include "Synth.h"
#include <stdlib.h>


Synth create_Synth(float freq,int sample_rate){
    Synth s = malloc(sizeof(struct Synth));
    s->shifter = (freq/((float)sample_rate))*(M_2PI);
    s->phi = 0;
    s->prev = 0;
    s->cur = 0;
    s->rate = (M_2PI/((float)sample_rate));
    return s;
}

void free_Synth(Synth s){
    free(s);
}
void reset_Synth(Synth s){
    s->phi = 0;
    s->prev = 0;
}

int cross_Synth(Synth s){
    if( s->prev <0 && s->cur >0 )
        return 1;
    if(s->prev >0 && s->cur <0)
        return 1;
    if(s->cur == 0)
        return 1;
    return 0;
}

void rewind_Synth(Synth s){
    s->phi -= s->shifter;
    if(s->phi < M_2PI){
        s->phi += M_2PI;
    }
}

double next_Sval(Synth s,double phase){
    s->phi += s->shifter;
    if(s->phi > M_2PI){
        s->phi -= M_2PI;
    }
    s->prev = s->cur;
    s->cur = sin(s->phi + phase);
    return s->cur;
}
double next_Sval_FM(Synth s,double fshift){
    s->phi += (s->shifter+(fshift*s->rate));
    if(s->phi > M_2PI){
        s->phi -= M_2PI;
    }
    s->prev = s->cur;
    s->cur = sin(s->phi);
    return s->cur;
}

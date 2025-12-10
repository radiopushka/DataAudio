#include "./DSP/DSP.h"
#include <stdio.h>
#include <stdlib.h>
#include "./alsa/alsa.h"
#include "DSP/FFT/FFT.h"
#include <math.h>

int main(){

    int rate = 48000;
    int samp_f = 8100;
    int bsize = rate/samp_f;

    short* frame = malloc(sizeof(short)*bsize);
    float* framec = malloc(sizeof(float)*bsize);

    if(setup_alsa("default",0,bsize,48000)<0)
         return 0;

    float freq[] = {20000.0f};

    FFT demod = create_FFT(freq,1,10,rate,rate);
    double f_conv_mult = (samp_f/4.0)/(M_PI/2);
    double avg = 0;
    int count = 0;
    while(1){
         aread(frame,bsize);
        float* cnvp = framec;
        for(short* sp = frame;sp<frame+bsize;sp++){
            *cnvp = (float) *sp;
            cnvp++;
        }

         double* fm = calculate_FFT_FM(demod, framec,  bsize);
         double f_hz = fm[0]*f_conv_mult;
         avg = avg + f_hz;
         count++;
         if(count >= 200){
            printf("%f\n",avg/200);
            avg = 0;
            count = 0;
         }
      }




    free_FFT(demod);
     free(frame);
     free(framec);
     free_alsa();
     return 0;


}

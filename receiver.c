#include "./alsa/alsa.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>

#include "./DSP/DSP.h"
#include "DSP/PLL/PLL.h"

int chr_extrct(unsigned char first,int samples){
    if(samples >2){
        return first;
    }else{
        return -1;
    }
}

int main(int argn,char* argv[]){
    int rate = 48000;
    int sample_rate = rate/80;//sampling freq must be x4 tx freq
    double sf = 44.0f;//Hz spreading factor
    float startf =18000.0f;


    short* frame = malloc(sizeof(short)*sample_rate);
    float* framec = malloc(sizeof(float)*sample_rate);




     if(setup_alsa("default",0,sample_rate*2,rate)<0)
         return 0;

     const int m_freq = 64;
     float freqs[m_freq];
     for(int i = 0;i<m_freq;i++){
         freqs[i]=startf + (sf * i);
         //printf("%f ",freqs[i]);
     }
     //printf("\n");
     FFT demod = create_FFT(freqs,m_freq,10,rate,rate);
    //int PLL_on = 1;//Later when I figure out short preamble


    //shift in array of characters
    unsigned char buffer[4];
    //clear the buffer at correct times
    unsigned int sample_clock = 0;



        while(1){
         aread(frame,sample_rate);
         float* m = framec;
         for(short* sp = frame;sp<frame+sample_rate;sp++){
            *m = (float)*sp;
            m++;
         }
             double* amps = calculate_FFT(demod,framec,sample_rate);
             double mval = 0;
             int m_index = 0;
             for(int i = 0;i<m_freq;i++){
                double fval = amps[i];
                if(fval > mval){
                    mval = fval;
                    m_index = i;
                }
             }
             unsigned char prev = *buffer;
             memmove(buffer+1, buffer, 3);
             *buffer = m_index;
             sample_clock++;

             //extract the actual character
             int has_char = -1;
             if(sample_clock >= 4){
                has_char = chr_extrct(buffer[1],sample_clock);
                 sample_clock = 0;
                 memset(buffer,0,4);
             }else if(prev != m_index && prev!=0){
                has_char = chr_extrct(buffer[1],sample_clock);
                sample_clock = 0;
                memset(buffer,0,4);
             }
             if(has_char != -1){
                printf("int value: %c %d\n",has_char,has_char);
             }

         }

    free_FFT(demod);

     free(frame);
     free(framec);
     free_alsa();
     return 0;

}


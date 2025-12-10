#include "./alsa/alsa.h"
#include<stdlib.h>
#include<math.h>
#include<stdio.h>

#include "./DSP/DSP.h"
#include "DSP/Synth/Synth.h"


int scross(float prev,float this){
    if(prev <0 && this >0)
        return 1;
    if(this == 0)
        return 1;
    if(this <0 && prev>0)
        return 1;
    return 0;
}

void set_value(char* pattern,short input){
            short value = input;
            pattern[0]=(value&(1<<5))? 1 :0;
            pattern[1]=(value&(1<<4))? 1 :0;
            pattern[2]=(value&(1<<3))? 1 :0;
            pattern[3]=(value&(1<<2))? 1 :0;
            pattern[4]=(value&(1<<1))? 1 :0;
            pattern[5]=(value&(1))? 1 :0;

}
//data packet:
//{6bstart_data,6bstart_data,4*6b 3 chars}
//last char is CRC

int main(int argn,char* argv[]){
    int rate = 48000;
    int chirp_frame = rate/20;
    double sf = 44.0f;//Hz spreading factor
    double startf =18000.0f;
    double amp_compensation = 1.0f;//higher frequencies are attenuated by speakers
                                   //set this value so that max and min are equal in amplitude



    short* frame = malloc(sizeof(short)*chirp_frame);

     if(setup_alsa(0,"default",rate,rate)<0)
         return 0;

     Synth mid = create_Synth(startf,rate*2);


     double amp = 16000.0/((sf*63)*(amp_compensation-1.0));
     if(amp_compensation <= 1.0){
         amp = 16000.0;
     }


        int size = 13;
         unsigned char test[]={0,10,20,30,40,50,60,63,63,62,61,60,'\n'};


         int shift_ptrn = 0;
         double prev_phase = 0;
         double cnv_rate = 24;
         while(1){

            double phase = test[shift_ptrn] * sf;
            double step = (prev_phase - phase)/cnv_rate;
            for(int i = 0;i<chirp_frame;i++){

                          //split the transition into a few samples to avoid harmonics
                          if(i<cnv_rate){
                              phase = prev_phase - step*(i+1.0);
                          }

                          double m1 = next_Sval_FM(mid,phase);
                          double m2 = next_Sval_FM(mid,phase);


                          double gain = phase*amp_compensation;
                          frame[i] = (short)((m1+m2)*(amp + gain));

        }

        prev_phase = phase;
        shift_ptrn++;
        if(shift_ptrn >= size){
            shift_ptrn = 0;
        }


         awrite(frame,chirp_frame);

     }

     free_Synth(mid);
     free(frame);
     free_alsa();
     return 0;

}

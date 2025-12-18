#include "./M66FSK/M66FSK.h"
#include <stdio.h>
#include <stdlib.h>
#include "Hamming.h"
#include "./alsa/alsa.h"

int main(){

    M66FSK_D fskdemod = create_66fsk_demod(17000.0f,48000, 80.0f);

    int bsize = fsk_get_buffer_size_d(fskdemod);

    short* frame = malloc(sizeof(short)*bsize);

    if(setup_alsa("default",0,bsize,48000)<0)
         return 0;

    while(1){
         aread(frame,bsize);
         int rx = fsk_get_uint(fskdemod,frame);
         if(rx){
             for(int i = 0;i<rx;i++){
                unsigned int data = pop_fsk_uint(fskdemod);

                int error = 0;
                data =decHamming26(data,&error);
                printf("%c%c%c",(data>>16)&255,(data>>8)&255,data&255);
             }
         }
    }




    free_66fsk_mod_d(fskdemod);
     free(frame);
     free_alsa();
     return 0;

}

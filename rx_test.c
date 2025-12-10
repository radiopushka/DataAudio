#include "./CFSK/CFSK.h"
#include <stdio.h>
#include <stdlib.h>
#include "./alsa/alsa.h"

int main(){

    CFSK_demod fskdemod = create_cfsk_demod(18000.0f, 50.0f, 48000, 8);

    int bsize = get_demod_bsize(fskdemod);

    short* frame = malloc(sizeof(short)*bsize);

    if(setup_alsa("default",0,bsize,48000)<0)
         return 0;

    while(1){
         aread(frame,bsize);
         int rx = get_cfsk_data(fskdemod,frame);
         if(rx!=-1){
             char top = rx>>8;
             char btm = rx &255;
             printf("%c%c",top,btm);
         }
    }




    free_cfsk_demod(&fskdemod);
     free(frame);
     free_alsa();
     return 0;

}

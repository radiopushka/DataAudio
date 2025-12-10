#include "./CFSK/CFSK.h"
#include <stdlib.h>
#include "./alsa/alsa.h"
#include <string.h>

int main(){

    CFSK_mod fskmod = create_cfsk_mod(18000.0f, 50.0f, 48000, 8);

    int bsize = get_mod_bsize(fskmod);
    short* frame = malloc(sizeof(short)*bsize);

     if(setup_alsa(0,"default",bsize,48000)<0)
         return 0;

     char* string="Hello World!\n";
     int len = strlen(string);

     int c_count = 0;
     while(1){

         if(c_count >= len){
             c_count = 0;
         }

         char s1 = string[c_count];
         char s2 = 0;
         if(c_count +1 < len){
            s2 = string[c_count+1];
         }
         unsigned short data = (s1<<8)|s2;

         put_cfsk_data(fskmod, data, frame);
         awrite(frame,bsize);
         c_count = c_count +2;
     }


     free_cfsk_mod(&fskmod);
     free(frame);
     free_alsa();
     return 0;

}

#include "./M66FSK/M66FSK.h"
#include <stdlib.h>
#include "./alsa/alsa.h"
#include <string.h>

int main(){

    M66FSK_M fskmod = create_66fsk_mod(18000.0f, 48000,70.0f);

    int bsize = fsk_get_buffer_size_m(fskmod);
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
         char s3 = 0;
         char s4 = 0;
         if(c_count +1 < len){
            s2 = string[c_count+1];
         }
         if(c_count +2 < len){
            s3 = string[c_count+2];
         }
         if(c_count +3 < len){
            s4 = string[c_count+3];
         }


         unsigned int data = (s1<<24)|(s2<<16)|(s3<<8)|s4;

         fsk_put_uint(fskmod, data, frame);
         awrite(frame,bsize);
         c_count = c_count +4;
     }


     free_66fsk_mod_m(fskmod);
     free(frame);
     free_alsa();
     return 0;

}

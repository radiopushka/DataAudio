#include "Hamming.h"

unsigned int _xor_all(unsigned int input){

    unsigned int out = BIT(input,0);
    for(int i = 1;i<31;i++){
        out = out ^ BIT(input,i);
    }
    return out;
}

unsigned int Hamming26(unsigned int input){
    unsigned int p1 = BIT(input,0) ^ BIT(input,1) ^ BIT(input,3) ^
        BIT(input,4) ^ BIT(input,6) ^ BIT(input,8) ^ BIT(input,10) ^
        BIT(input,11) ^ BIT(input,13) ^ BIT(input,15) ^ BIT(input,17) ^
        BIT(input,19) ^ BIT(input,21) ^ BIT(input,23) ^ BIT(input,25);
   unsigned int p2 = BIT(input,0) ^ BIT(input,2) ^ BIT(input,3) ^
        BIT(input,5) ^ BIT(input,6) ^ BIT(input,9) ^ BIT(input,10) ^
        BIT(input,12) ^ BIT(input,13) ^ BIT(input,16) ^ BIT(input,17) ^
        BIT(input,20) ^ BIT(input,21) ^ BIT(input,24) ^ BIT(input,25);
   unsigned int p3 = BIT(input,1) ^ BIT(input,2) ^ BIT(input,3) ^
        BIT(input,7) ^ BIT(input,8) ^ BIT(input,9) ^ BIT(input,10) ^
        BIT(input,14) ^ BIT(input,15) ^ BIT(input,16) ^ BIT(input,17) ^
        BIT(input,22) ^ BIT(input,23) ^ BIT(input,24) ^ BIT(input,25);
   unsigned int p4 = BIT(input,4) ^ BIT(input,5) ^ BIT(input,6) ^
        BIT(input,7) ^ BIT(input,8) ^ BIT(input,9) ^ BIT(input,10) ^
        BIT(input,18) ^ BIT(input,19) ^ BIT(input,20) ^ BIT(input,21) ^
        BIT(input,22) ^ BIT(input,23) ^ BIT(input,24) ^ BIT(input,25);
   unsigned int p5 = BIT(input,11) ^ BIT(input,12) ^ BIT(input,13) ^
        BIT(input,14) ^ BIT(input,15) ^ BIT(input,16) ^ BIT(input,17) ^
        BIT(input,18) ^ BIT(input,19) ^ BIT(input,20) ^ BIT(input,21) ^
        BIT(input,22) ^ BIT(input,23) ^ BIT(input,24) ^ BIT(input,25);

   unsigned int val = p1|(p2<<1)|((input&1)<<2)|(p3<<3)|(((input>>1)&7)<<4)|
       (p4<<7)|(((input>>4)&127)<<8)|(p5<<15)|((input>>11)<<16);

   unsigned int p6 = _xor_all(val);
   val = val | (p6<<31);


   return val;

}
unsigned int decHamming26(unsigned int input_p,int* ecode){

    unsigned int rp1 = BIT(input_p,0);
    unsigned int rp2 = BIT(input_p,1);
    unsigned int rp3 = BIT(input_p,3);
    unsigned int rp4 = BIT(input_p,7);
    unsigned int rp5 = BIT(input_p,15);
    unsigned int rp6 = BIT(input_p,31);

    unsigned int p6 = _xor_all(input_p);

    unsigned int parity_extract = (input_p>>2)&1;
    unsigned int frag2 = (input_p>>4)&7;
    unsigned int frag3 = ((input_p)>>8)&127;
    unsigned int trm = input_p&(~(1<<31));
    unsigned int frag4 = (trm>>16);
    unsigned int input = parity_extract|(frag2<<1)|(frag3<<4)|(frag4<<11);


    unsigned int p1 = BIT(input,0) ^ BIT(input,1) ^ BIT(input,3) ^
        BIT(input,4) ^ BIT(input,6) ^ BIT(input,8) ^ BIT(input,10) ^
        BIT(input,11) ^ BIT(input,13) ^ BIT(input,15) ^ BIT(input,17) ^
        BIT(input,19) ^ BIT(input,21) ^ BIT(input,23) ^ BIT(input,25);
    unsigned int p2 = BIT(input,0) ^ BIT(input,2) ^ BIT(input,3) ^
        BIT(input,5) ^ BIT(input,6) ^ BIT(input,9) ^ BIT(input,10) ^
        BIT(input,12) ^ BIT(input,13) ^ BIT(input,16) ^ BIT(input,17) ^
        BIT(input,20) ^ BIT(input,21) ^ BIT(input,24) ^ BIT(input,25);
    unsigned int p3 = BIT(input,1) ^ BIT(input,2) ^ BIT(input,3) ^
        BIT(input,7) ^ BIT(input,8) ^ BIT(input,9) ^ BIT(input,10) ^
        BIT(input,14) ^ BIT(input,15) ^ BIT(input,16) ^ BIT(input,17) ^
        BIT(input,22) ^ BIT(input,23) ^ BIT(input,24) ^ BIT(input,25);
    unsigned int p4 = BIT(input,4) ^ BIT(input,5) ^ BIT(input,6) ^
        BIT(input,7) ^ BIT(input,8) ^ BIT(input,9) ^ BIT(input,10) ^
        BIT(input,18) ^ BIT(input,19) ^ BIT(input,20) ^ BIT(input,21) ^
        BIT(input,22) ^ BIT(input,23) ^ BIT(input,24) ^ BIT(input,25);
    unsigned int p5 = BIT(input,11) ^ BIT(input,12) ^ BIT(input,13) ^
        BIT(input,14) ^ BIT(input,15) ^ BIT(input,16) ^ BIT(input,17) ^
        BIT(input,18) ^ BIT(input,19) ^ BIT(input,20) ^ BIT(input,21) ^
        BIT(input,22) ^ BIT(input,23) ^ BIT(input,24) ^ BIT(input,25);

    unsigned int syndrome = (p1 ^ rp1)|((p2 ^ rp2)<<1)|((p3 ^ rp3)<<2)|((p4 ^ rp4)<<3)|((p5 ^ rp5)<<4);


    if(syndrome == 0){
        *ecode = 0;
        if(p6 !=  rp6){
            *ecode = 2;
        }


        return input;
    }

    if(syndrome>25){
        *ecode = 3;
        return 0;
    }

    if(p6 != rp6){
        unsigned int bad_bit = BIT(input,syndrome) ^ 1;
        input &= ~(1<<syndrome);
        input |= bad_bit<<syndrome;


        *ecode = 1;
        return input;
    }

    *ecode = 3;
    return 0;

}

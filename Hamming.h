#ifndef HAMMING_H
#define HAMMING_H

#define BIT(data,i) (data>>i)&1

unsigned int Hamming26(unsigned int input);
unsigned int decHamming26(unsigned int input_p,int* ecode);

#endif

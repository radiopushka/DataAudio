#ifndef _66FSK
#define _66FSK
#include "../DSP/DSP.h"
//another modulation method of transmitting data
//it consists of 33 simultaneous carriers
//the 66 frequencies are broken up into pairs of two
//for example, the first two frequencies  will have a carrier in
//the lower to indicate a zero and in the upper to indicate a one
//hence you can send 33 bits at a time
//the first bit will be the clock, this is for data integrity
//the change rate will be at most 8 times per second, the slower the better;
//less errors.
//main disadvantage: TPO is divided amongst 33 frequencies
//32*8 -> 256 bits per second
//this is not a form of OFDM modulation
//Evan Nikitin 2025
//Vostok Telecom
//Russian Federation
//
//бинарная частотная модуляция: с 33мя несущями, несущия может переключатся
//между своей нижний и своей верхний частотой с коэффицентом смегчения.
//длительное время на нижний частоте обозначает ноль, длительное время на
//верхний обозночает один.Таких груп будет всего 33, значит мы можем передовать
//33 бита в каждом интервале. Нулевой бит обеспечивает синхронизацию, в
//итоге получается 32 бита в интервал. Наш интервал должен быть достаточно большой,
//когда отражений много то нужно больше времени для стабилизаций сигнала у приемника.
//Это основная проблема передатчи информаций через звук, он медленый, всего 300 метров в секунду в воздухе.
//на данный момент, интервал будет зафиксирован как 8 раз в секундну.
//При таком интервали скорость передатчи будет в два раза быстрее чем наш CFSK в хороших условиях.
//Наш CFSK уже довольно стабильный при этом интервале;
//в мальенкой комнате где отсутсвует какой либo магкий материал, ошибок не так много но они все ровно есть

struct M66FSK_m{
    //two carriers for clock for redundancy
    Synth gen[34];
    double lpf[34];
    double sf;
    double alpha;

    int sample_size;
    int clock;
};
struct M66FSK_d{
    FFT demod;
    double r_avg[64];
    unsigned int data_stack[4];

    int clock;
    int sample_size;
    int samp_freq;
    int samp_mult;
    float* demod_list;
    double samp_count;


};
typedef struct M66FSK_m* M66FSK_M;
typedef struct M66FSK_d* M66FSK_D;

M66FSK_M create_66fsk_mod(double start_freq,int sample_rate, double sf);
M66FSK_D create_66fsk_demod(double start_freq,int sample_rate,double sf);

int fsk_get_buffer_size_m(M66FSK_M m);
int fsk_get_buffer_size_d(M66FSK_D d);

void fsk_put_uint(M66FSK_M m,unsigned int data,short* buffer);
//returns the number of data fragments captured: run pop to retrieve
unsigned int fsk_get_uint(M66FSK_D d,short* buffer);
unsigned int pop_fsk_uint(M66FSK_D d);

void free_66fsk_mod_m(M66FSK_M m);
void free_66fsk_mod_d(M66FSK_D d);

#endif

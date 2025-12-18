#ifndef FRAME_H
#define FRAME_H

#include <alsa/asoundlib.h>
#include <pthread.h>
#include "./M66FSK/M66FSK.h"

struct Transmitter{
    snd_pcm_t *pcm;
    snd_pcm_status_t* stat;
    pthread_t thread;

    short* data;

    M66FSK_M fskmod;
    int bsize;

    pthread_mutex_t data_mutex;
    unsigned int datad;
    int status;

    pthread_mutex_t sleep_mutex;
};
struct Receiver{
    snd_pcm_t *pcm;
    snd_pcm_status_t* stat;
    pthread_t thread;

    short* data;

    M66FSK_D fskdemod;
    int bsize;

    pthread_mutex_t data_mutex;
    int status;
    unsigned int datad;

    pthread_mutex_t sleep_mutex;
};
struct Transceiver{
    Transmitter t;
    Receiver r;
};

/*
 lock data mutex and put int data and status ready
 unlock data_mutex
 lock sleep_mutex
 worker thread will unlock the sleep mutex
 lock and unlock the sleep mutex after in main
 */

#endif

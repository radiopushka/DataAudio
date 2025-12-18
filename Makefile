
FLAGS = -march=native -mtune=native -O3 -funroll-loops -fomit-frame-pointer -flto -fno-signed-zeros -fno-trapping-math -fno-fast-math
DSP = ./DSP/FFT/FFT.c ./DSP/PLL/PLL.c ./DSP/Synth/Synth.c ./Hamming.c
CFSK = ./CFSK/CFSK.c
M66FSK = ./M66FSK/M66FSK.c

all:chirp m66fsk lib

m66fsk:
	$(CC) ./tx_test2.c $(FLAGS) $(M66FSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigen6
	$(CC) ./rx_test2.c $(FLAGS) $(M66FSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigrx6
chirp:
	$(CC) ./tx_test.c $(FLAGS) $(CFSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigen
	$(CC) ./rx_test.c $(FLAGS) $(CFSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigrx
lib:
	$(CC) $(M66FSK) $(CFSK) $(FLAGS) $(DSP) -lm -fPIC -shared -o dsp.o




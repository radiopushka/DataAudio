
FLAGS = -march=native -O3
DSP = ./DSP/FFT/FFT.c ./DSP/PLL/PLL.c ./DSP/Synth/Synth.c
CFSK = ./CFSK/CFSK.c
M66FSK = ./M66FSK/M66FSK.c

all:
#	$(CC) ./transmitter.c $(FLAGS) $(DSP) ./alsa/alsa.c -lm -lasound -o sigen
#	$(CC) ./receiver.c $(FLAGS) $(DSP) ./alsa/alsa.c -lm -lasound -o sigrx
	$(CC) ./tx_test.c $(FLAGS) $(CFSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigen
	$(CC) ./rx_test.c $(FLAGS) $(CFSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigrx

	$(CC) ./tx_test2.c $(FLAGS) $(M66FSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigen6
	$(CC) ./rx_test2.c $(FLAGS) $(M66FSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigrx6

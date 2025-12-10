
FLAGS = -march=native -O3
DSP = ./DSP/FFT/FFT.c ./DSP/PLL/PLL.c ./DSP/Synth/Synth.c
CFSK = ./CFSK/CFSK.c

all:
#	$(CC) ./transmitter.c $(FLAGS) $(DSP) ./alsa/alsa.c -lm -lasound -o sigen
#	$(CC) ./receiver.c $(FLAGS) $(DSP) ./alsa/alsa.c -lm -lasound -o sigrx
	$(CC) ./tx_test.c $(FLAGS) $(CFSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigen
	$(CC) ./rx_test.c $(FLAGS) $(CFSK) $(DSP) ./alsa/alsa.c -lm -lasound -o sigrx
	$(CC) ./IRQ_demod.c $(FLAGS) $(DSP) ./alsa/alsa.c -lm -lasound -o irq

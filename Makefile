COMPILER = gcc

all: main.c
	$(COMPILER) -o main main.c -lcurl

clean:
	rm main
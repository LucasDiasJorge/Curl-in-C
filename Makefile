COMPILER = gcc

curl: main.c
	$(COMPILER) -o main main.c -lcurl

clean:
	rm main
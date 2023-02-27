COMPILER = gcc

curl: main.c
	$(COMPILER) -o main main.c -lcurl -lcjson

clean:
	rm main

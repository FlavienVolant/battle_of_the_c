
main: src/main.c src/server.c src/battleship.c
	gcc -o main src/main.c src/server.c src/battleship.c -Iinclude -lws2_32

run: main
	./main.exe

clear:
	rm main.exe
	
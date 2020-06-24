#Makefile del trabajo practico

CC=g++ # Compilador
CFLAGS=-Wall -pedantic #Flag que muestra todos los errores

all: main

main: main.o Token.o functions.o stringUtils.o Utils.o Complejo.o Pixel.o PixelArray.o img.o cmdline.o benchmark_propio.o Fractal.o JuliaFractal.o
	$(CC) $(CFLAGS) Token.o functions.o stringUtils.o Utils.o main.o Complejo.o PixelArray.o img.o Pixel.o cmdline.o benchmark_propio.o Fractal.o JuliaFractal.o -o main

main.o: main.cpp main.h img.h cmdline.h  Fractal.h JuliaFractal.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

Complejo.o: Complejo.cpp Complejo.h
	$(CC) $(CFLAGS) -c Complejo.cpp -o Complejo.o

Pixel.o: Pixel.cpp Pixel.h Complejo.h
	$(CC) $(CFLAGS) -c Pixel.cpp -o Pixel.o

PixelArray.o: PixelArray.cpp PixelArray.h Pixel.h 
	$(CC) $(CFLAGS) -c PixelArray.cpp -o PixelArray.o

img.o: img.cpp img.h PixelArray.h
	$(CC) $(CFLAGS) -c img.cpp -o img.o

cmdline.o: cmdline.cpp cmdline.h
	$(CC) $(CFLAGS) -c cmdline.cpp -o cmdline.o

Token.o: Token.cpp Token.h Complejo.h functions.h  tokenDictionary.h
	$(CC) $(CFLAGS) -c Token.cpp -o Token.o

Utils.o: Utils.cpp Utils.h Stack.h Token.h 
	$(CC) $(CFLAGS) -c Utils.cpp -o Utils.o

stringUtils.o: stringUtils.cpp stringUtils.h Stack.h
	$(CC) $(CFLAGS) -c stringUtils.cpp -o stringUtils.o

functions.o: functions.cpp functions.h Complejo.h
	$(CC) $(CFLAGS) -c functions.cpp -o functions.o

benchmark_propio.o: benchmark_propio.h
	$(CC) $(CFLAGS) -c benchmark_propio.cpp -o benchmark_propio.o

Fractal.o: img.h
	$(CC) $(CFLAGS) -c Fractal.cpp -o Fractal.o

JuliaFractal.o: Fractal.h img.h
	$(CC) $(CFLAGS) -c JuliaFractal.cpp -o JuliaFractal.o

clean:
	rm *.o


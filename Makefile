build:
	gcc -Wall -Wcomment -std=c99 ./src/*.c -lSDL2 -lm -o renderer

run:
	./renderer 

clean:
	rm renderer

	
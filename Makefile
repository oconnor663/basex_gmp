base62: base62.c
	gcc base62.c -o base62 -lgmp -lm -O2 -Wall -pedantic

test: base62
	./test.sh

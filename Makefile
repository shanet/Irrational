BINARY=irrational

machin:
	gcc -std=c99 -o $(BINARY) -Wall -Wextra -O3 machin.c -lmpfr -pthread

recurrance:
	gcc -std=c99 -o $(BINARY) -Wall -Wextra -O3 recurrance.c -lmpfr

test:
	./$(BINARY) --hide-pi 1000000

clean:
	rm -f $(BINARY)

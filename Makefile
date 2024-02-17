build: 
	gcc -Wall -g lista.c tema2.c -o tema2
run:
	./tema1 10 file.in file.out
clean:
	rm -rf *.o tema2
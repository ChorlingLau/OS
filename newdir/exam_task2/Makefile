all:
	cd code && make
	gcc -o fibo code/fibo.o code/main.o
.PHONY: clean
clean:
	cd code && make clean
	rm fibo

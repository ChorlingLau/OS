all: hello_os.c
	gcc hello_os.c -o os_hello
.PHONY: clean
clean:
	rm os_hello

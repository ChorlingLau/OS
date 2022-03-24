INCLUDES := -I../readelf/
CC        := gcc


%.o: %.c
	$(CC) $(INCLUDES) -c $<

.PHONY: clean

readelf: main.o readelf.o 
	$(CC) -g main.o readelf.o -o readelf


clean:
	rm -rf *.o
	rm readelf

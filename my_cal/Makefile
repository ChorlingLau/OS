INCLUDES      := -I../include/

# ======== End of configuration ===


all: my_cal.c my_driver.S
	$(CC) $(INCLUDES) -c my_cal.c -o my_cal.o
	$(CC) $(INCLUDES) -c my_driver.S -o my_driver.o

include ../include.mk

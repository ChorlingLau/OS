/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>
#define SIZE_C 100
typedef struct {
	int size;
	char c;
	int array[SIZE_C];
}my_struct;

int main()
{
	printf("main.c:\tmain is start ...\n");
	my_struct ms = {2, 'a', {4, 5}};
	printf("%T\n", &ms);
	printf("%-T\n", &ms);
	printf("%5T\n", &ms);
	printf("%04T\n", &ms);
	mips_init();
	panic("main is over is error!");

	return 0;
}

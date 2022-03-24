#include "add.h"

int main() { 
	int num = 1, i, res = 0;
	for (i = 0; i < 5; i++) {
		res = add(res, num++);
	}
	return 0; 
}

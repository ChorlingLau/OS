
char _my_getchar();
void _my_putchar(char c);
void _my_exit();

void my_cal() {
	char c;
	unsigned int num=0;
	c = _my_getchar();
	while(c!='\n') {
		num = num * 10 + c - '0';
		c = _my_getchar();
	}
	int a[100]={0};
	int len=0;
	while(num){
		a[len++]= num % 2;
		num = num / 2;
	}
	while(len) {
		c = a[len-1]+'0';
		_my_putchar(c);
	}
}

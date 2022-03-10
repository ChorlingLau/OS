#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	int len = 0;
	int num[20] = {0};
	while(n) {
		num[len++] = n % 10;
		n = n / 10;
	}
	int start = 0, end = len - 1, flag = 0;
	while(start < end) {
		if (num[start] != num[end]) {
			flag = 1;
			break;
		}
		start++;
		end--;
	}
	if(!flag){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}

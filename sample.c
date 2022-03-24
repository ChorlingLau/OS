int add(int a, int b) {
	return a + b;
}

int main()
{ 
	int num = 1, i, res = 0;
	for (i = 0; i < 5; i++) {
		res += num++;
	}
	return 0; 
}

// #include <stdio.h>
void print(int a[10]) {
	int i = 0;
	float f;
	f =   1+.4;
	for(i = 0; i < 11; i = i + 1) {
		printf("%d ", a[i]);
	}
	printf("\n");
}
int main() {
	int a[10] = {1, 2};
	print(a);
}

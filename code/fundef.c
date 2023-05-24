int main() {
	return 0;
}

int fun(int i) {
	if(i == 1)
		return 1;
	else if (i == 2)
		return 2;
	return f(i-1) + f(i-2);
}
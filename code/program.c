int a[25][25];
int b[25][25];
int n, m, p;

void mat_mul(int r[25][25], int a[25][25], int b[25][25]) {
	int i, j, k;
	for(i = 0; i < m; i = i + 1) {
		for(j = 0; j < n; j = j + 1) {
			for(k = 0; k < n; k = k + 1){
				r[i][j] = r[i][j] + a[i][k] * b[k][j];
			}
		}
	}
}
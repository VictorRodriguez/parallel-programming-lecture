#define MAX 1000000
#include <stdio.h>

int a[256], b[256], c[256];
__attribute__((target_clones("avx2","avx512f","default")))

void foo(){
	int i,j;
	for (j=0;j<MAX;j++){
		for (i=0; i<256; i++){
			a[i] = b[i] + c[i];
		}
	}
}

int main () {
	foo();
	return 0;
}

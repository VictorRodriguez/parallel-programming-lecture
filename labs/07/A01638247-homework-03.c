#include <stdio.h>
#include <omp.h>
#include <math.h>

//Define the function
#define f(x) sin(x)

int main() {
	//Variables
	int threads = 8;
	float k;
	int numLower = 0;
	int numUpper = 1;
	int interval = 10;
	int i;
	
	//Number of the step size
	float step = (float)(numUpper-numLower) / interval;
	
	//Integration variables
	float integ = f(numLower) + f(numUpper);
	
	//Pragma declaration
	# pragma omp parallel num_threads(threads) shared(integ, k) private(i) {
		#pragma omp for 
			for (i = 0; i <= interval; i++) {
				k = numLower + i * step;
				integ = integ + 2 * f(k);
			}
	}
	
	//Integration values
	integ = integ * step / 2;
	printf("Area under the curve approximate: %.4f\n", integ);
	return 0;
}

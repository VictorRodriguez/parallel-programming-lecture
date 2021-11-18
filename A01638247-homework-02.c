#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void seller1(int count, int *bottles) {
	if (count > *bottles) {
		printf("You cant sell %d bottles, because there were %d bottles remaining\n", count, *bottles);
		
	} else {
		*bottles = *bottles - count;
		printf("Seller 1 sell %d, %d bottles remaining\n", count, *bottles);
	}
}

void seller2(int count, int *bottles) {
	if (count > *bottles) {
		printf("You cant sell %d bottles, because there were %d bottles remaining\n", count, *bottles);
	} else {
		*bottles = *bottles - count;
		printf("Seller 2 sell %d, %d bottles remaining\n", count, *bottles);
	}
}
void seller3(int count, int *bottles) {
	if (count > *bottles) {
		printf("You cant sell %d bottles, because there were %d bottles remaining\n", count, *bottles);
	} else {
		*bottles = *bottles - count;
		printf("Seller 3 sell %d, %d bottles remaining\n", count, *bottles);
	}
}
void seller4(int count, int *bottles) {
	if (count > *bottles) {
		printf("You cant sell %d bottles, because there were %d bottles remaining\n", count, *bottles);
	} else {
		*bottles = *bottles - count;
		printf("Seller 4 sell %d, %d bottles remaining\n", count, *bottles);
	}
}
void seller5(int count, int *bottles) {
	if (count > *bottles) {
		printf("You cant sell %d bottles, because there were %d bottles remaining\n", count, *bottles);
	} else {
		*bottles = *bottles - count;
		printf("Seller 5 sell %d, %d bottles remaining\n", count, *bottles);
	}
}
int main () {
	srand(time(NULL));
	int bottles = 1000;
	int count;
	int n;
	do{
		printf("Seller number: ");
		scanf("%d", &n);
    count = rand()%500+1;
		switch(n) {
			case 1:
				seller1(count, &bottles);
				break;
			case 2:
				seller2(count, &bottles);
				break;
			case 3:
				seller3(count, &bottles);
				break;
			case 4:
				seller4(count, &bottles);
				break;
			case 5:
				seller5(count, &bottles);
				break;
			default:
        printf("Incorrect number of seller\n");
				break;
		}
	} while (bottles > 0);
	printf("Theres no bottles remaining");
	return 0;
}
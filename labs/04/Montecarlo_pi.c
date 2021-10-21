/*Link to graphic: https://drive.google.com/file/d/19_CU6RRLfPZMx4S7uhCExiOO8PfUqfZ5/view?usp=sharing */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define DEFAULT_POINTS      10000
#define NUM_THREADS         16

long    circle_count;        //points inside the circle
unsigned int seed;           //used for rand_r
int    pointsPerThread;		 //points to be proccessed by each thread

//randomizer from 0 to 1
double r2(){
    return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

//Montecarlo calculation
void* montecarlo(void *val){
	int *counter = (int *)malloc(sizeof(int));
	int i;
	for(i=0; i<pointsPerThread; i++){
		double x = r2();
		double y = r2();
		double root = sqrt(pow((x-.5),2)+ pow((y-.5),2));
		if(root<.5){
			*counter += 1;
		}
	}
	pthread_exit((void *) counter);

}

int main(int argc, const char* argv[]){

    // time count start
	clock_t start = clock();
	seed = time(NULL);

	//obtaining and using the values from global variables 
	long points = DEFAULT_POINTS;
	long nThreads = NUM_THREADS;
	pointsPerThread = points/nThreads;
	pthread_t threads[nThreads];

	//Thread count used for pthread_join
	void    *thread_count;

    //Variable to store created threads
    int     current_thread;

	unsigned int i;

    //Cycle for creating threads
	for(i=0; i<nThreads; i++){
		current_thread = pthread_create(&threads[i],NULL,montecarlo,(void *)(intptr_t) i);
		seed++;
		current_thread ? exit(1) : printf("Thread %i done\n", i);
	}

    //Cycle for joining threads
	for(i=0; i<nThreads; i++){
        	pthread_join(threads[i], &thread_count);
        	circle_count += * (long *) thread_count;
        } 

        printf("\nPoints deployed: %zd\n"
        "\nPoints inside circle: %zd\n", points, circle_count);

        //pi calculation and print
        float pi = (4.00 * (float)circle_count) / (float)points;
        printf("\nPi-> %.5f\n", pi);

        //time count end and print
        clock_t end = clock();
    	double time = (double)(end-start) / CLOCKS_PER_SEC;
    	printf("\nCalculation took %.3f\n",time);

exit(0);
}

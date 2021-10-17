/*
Link a grafica: 
https://docs.google.com/spreadsheets/d/193QjARyWOg6bN4Dingg4aaYqNGzAMB6qSqdAcy1Kqfw/edit?usp=sharing

Autor:
Andres Remis Serna A00829053

Con apoyo de los siguientes repositorios:
https://github.com/VictorRodriguez/operating-systems-lecture/blob/master/labs/04/pi.c
https://github.com/VictorRodriguez/operating-systems-lecture/blob/master/labs/04/mutex-thread.c
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS   75

int npoints = 100000;
int circle_count = 0;
double radius=1;
pthread_mutex_t lock;
int pointsThread;


/*https://github.com/VictorRodriguez/operating-systems-lecture/blob/master/labs/04/pi.c */

unsigned int seed;

double r2(){
    return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

/*fin github*/

int find_circle(double x, double y){

    double d = pow(radius,2) - (pow(x,2) + pow(y,2));

    if(d>=0){
        return 1;
    }else{
        return 0;
    }
}

void * Count(void * a){
    int cont=0;
    pthread_mutex_lock(&lock);
    for (int j=1; j<=pointsThread; j++){
        //generate 2 random numbers between 0 and 1
        double xcoordinate = r2();
        double ycoordinate = r2();
        int circ=find_circle(xcoordinate,ycoordinate);
        if (circ==1) {
            circle_count = circle_count + 1;
        }else{
            cont++;//end do
        }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]){
    clock_t start, end;
    double cpu_time_used;
     
    start = clock();
    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
            return 1;
    }
    //do j = 1,npoints
    seed = time(NULL);

    pthread_t threads[NUM_THREADS];
    pointsThread= npoints/NUM_THREADS;
    
    
    for(int i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, Count, NULL);
    }

    for(int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    double PI = 4.0*(double)circle_count/(double)npoints;
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("PI = %f\nTime used=%f",PI, cpu_time_used);
    pthread_mutex_destroy(&lock);
}
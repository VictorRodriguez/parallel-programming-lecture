/*
    Kevin Vega Rodriguez - A01612430
    Drive : https://drive.google.com/file/d/1CO46euCwzNY09k2fnf8jr1XIez-S4rx5/view?usp=sharing
    
    Logic for me is OK but for some reason with more threads, more execution time, debugging 
    with some prints looks like the threads are being created and doing the less iterations when
    more threads are available, but the time execution is bigger
    I try the code using Winows Subsystem for linux in a Ubuntu distribution

    Compiling:
    gcc pi-calculation.c -o pi-calculation -lm -pthread
    running:
    ./pi-calculation <threads>

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int THREADS = 2;
#define TOTAL_I 10000000
//int ntr = 0;

float randNumGen(){ // Creating a random value

   int random_value = rand(); //Generate a random number   
   float unit_random = random_value / (float) RAND_MAX; //make it between 0 and 1 
   return unit_random;
}

void *montecarlo(void *threadid){
    long  longTid; 
    longTid = (long)threadid;
    int tid = (int)longTid;

    //printf("Thread starting: %d\n",ntr);
    //ntr++;
    //print for debugging



    //As i understood we need to use malloc to make
    //sure tht our answer is not destroyed when the
    //thread ends
    long *times = (long *)malloc(sizeof(long));
    *times = 0;

    //We divide the iterations by thread
    int tot_it = TOTAL_I/THREADS;
    printf("%d\n",tot_it);
    for (int counter = 0; counter < tot_it; counter++)
    {
        // We generate 2 random coordinates
        float x = randNumGen();
        float y = randNumGen();
        float ans = sqrt((x*x)+(y*y));

        //Check if the random number is inside the circle
        if (ans<1){
            *times+=1;
        }   
    }
    //printf("Thread finished: %d\n",ntr);
    //ntr--;
    //print for debugging

    pthread_exit((void *)times);
}

int main(int argc, char const *argv[])
{

    char *p;
  
    long conv = strtol(argv[1], &p, 10);
    printf("%ld\n", conv);
    THREADS = conv;
    printf("%d\n",THREADS);
    

    pthread_t threads[THREADS]; 
    int rc;
    void *status;
    float inside=0;

    long t;
    for (t = 0; t < THREADS; t++)
    {
        rc = pthread_create(&threads[t],NULL,montecarlo,(void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        } 
    }

    for (t = 0; t < THREADS; t++)
    {
        pthread_join(threads[t], &status);
        inside+=*(long*)status;
    }
       
    printf("%f\n", (4*(inside/TOTAL_I)));

    pthread_exit(NULL);

    return 0;
}

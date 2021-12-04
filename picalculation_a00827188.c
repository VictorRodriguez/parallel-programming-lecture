#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>


int n_threads;
int p_thread;
long t_points = 1000000;
long i_points = 0;
unsigned int seed;
pthread_mutex_t lock;


double get_Number(){
    double num;
    num = (double) rand_r(&seed) / ((unsigned) RAND_MAX + 1);
    return num;
}


void* pi_cal(void* t_id){
    for (int j=0; j<p_thread; j++){
        double x = get_Number();
        double y = get_Number();
        if (sqrt(pow((x-0.5),2)+pow((y-0.5),2)) <= 0.5){
            pthread_mutex_lock(&lock);
            i_points++;
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}


int main(int argc, const char* argv[]){
    clock_t t;
    t = clock();
    seed = time(NULL);
    n_threads = atoi(argv[1]);
    p_thread = t_points / n_threads;
    pthread_t threads[n_threads];
    double pi;

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\nmutex init failed\n");
        return 1;
    }

    for (long i=0; i < n_threads; i++){
        if(pthread_create(&threads[i], NULL, pi_cal, (void*)i)){
            printf("\npthread create failed\n");
            return 1;
        }
    }
    for (int i=0; i < n_threads; i++){
        pthread_join(threads[i], NULL);
    }

    pi = (4.0 * i_points) / t_points;
    pthread_mutex_destroy(&lock);
    printf("\nPi = %lf\n", pi);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("\nTime taken: %lf\n", time_taken);
    pthread_exit(NULL);

    return 0;
}
#define SIZE 100000

int a[SIZE];
int b[SIZE];
int c[SIZE];

int foo(){

        int j;
        int i;

        for (j = 0; j < SIZE; j++){
                for (i = 0; i < SIZE; i++){
                        c[i] = a[i] + b[i];
                }
        }
        return 0;
}

int main(){

        int ret;
        ret = foo();
        return ret;

}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
    double r=0.5;
    double x=0.5;
    double y=0;

    double d = pow(r,2) - (pow(x,2) + pow(y,2));
    printf("d = %f \n", d);

    if(d>0){
        printf("inside");
    }else if(d==0){
        printf("on the circumference");
    }else{
        printf("outside");
    }
}
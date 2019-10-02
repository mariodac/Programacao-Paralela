#include "stdio.h"
#include "pthread.h"
void *func(void *arguments){
    pthread_exit(NULL);
}
int main(){
    int err = 0, n = 0;
    pthread_t t;
    while (err == 0)
    {
        err = pthread_create(&t, NULL, func, NULL);
        n++;
    }
    printf("Max threads: %d\n", n);
    
    return 0;
}
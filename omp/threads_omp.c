#include <omp.h>
#include <stdio.h>
void helloWorld(){
    #pragma omp parallel
    {
        if(omp_get_thread_num() == 0){
          
            printf("Total threads: %d\n", omp_get_num_threads());
        }
        else{
            printf("Hello %d\n", omp_get_thread_num());
            printf("World! %d\n", omp_get_thread_num());
        }
    }
}
int main(int argc, char const *argv[])
{
    // printf("Version: %d\n", _OPENMP);
    // omp_set_num_threads(2);
    // #pragma omp parallel //num_threads(20)
    helloWorld();
    return 0;
}

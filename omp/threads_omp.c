#include <omp.h>
#include <stdio.h>
void helloWorld(){
    int id_thread;
    #pragma omp parallel default(none) private(id_thread)
    {
        id_thread = omp_get_thread_num();
        if(id_thread == 0){
            printf("Total threads: %d\n", omp_get_num_threads());
        }
        else{
            printf("Hello %d\n", id_thread);
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

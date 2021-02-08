#include <time.h>
#include <math.h>
#include "openmp_reductions_declarations.c"
#include <stdlib.h>
#include <stdio.h>


int minabs_int_test(){
    srand(time(NULL)) ;
    int nb_tests = rand() % 10000 ; //max 1000 tests
    for(int i = 0; i < nb_tests ; i++){
        int data_size = rand() % 100000 ; //max size of the rarrays : 10000
        int * array = calloc(data_size, sizeof(int)) ;
        for(int j = 0; j < data_size; j++){
            // initialization of the array
            array[j] = (rand() - (RAND_MAX/2))*2 ; // between -INTMAX et INTMAX
        }
        int minabs_result_parallelized = __INT_MAX__ ; // initalization at the max because it's going to be the min ...
        #pragma omp parallel for reduction(minabs: minabs_result_parallelized)
        for(int j = 0 ; j < data_size ; j++){
            // computing the min value parallized
             minabs_result_parallelized = (abs(array[j]) < minabs_result_parallelized ? abs(array[j]) : minabs_result_parallelized) ;
        }
        int minabs_result_sequential = __INT_MAX__ ;
        for(int j = 0 ; j < data_size ; j++){
            // computing the min value sequential
             minabs_result_sequential = (abs(array[j]) < minabs_result_sequential ? abs(array[j]) : minabs_result_sequential) ;
        }
        free(array) ;
        if (minabs_result_parallelized != minabs_result_sequential)
            return 1 ;
    }
    // no error
    return 0 ;
}

int minabs_float_test(){
    srand(time(NULL)) ;
    int nb_tests = rand() % 10000 ; //max 1000 tests
    for(int i = 0; i < nb_tests ; i++){
        int data_size = rand() % 100000 ; //max size of the rarrays : 10000
        float * array = calloc(data_size, sizeof(float)) ;
        for(int j = 0; j < data_size; j++){
            // initialization of the array
            array[j] = (rand() - (RAND_MAX/2))/pow(10, rand()%9) ; // divided by sométhing between 1 and 10^9
        }
        float minabs_result_parallelized = __FLT_MAX__ ; // initalization at the max because it's going to be the min ...
        #pragma omp parallel for reduction(minabs: minabs_result_parallelized)
        for(int j = 0 ; j < data_size ; j++){
            // computing the min value parallized
             minabs_result_parallelized = (fabs(array[j]) < minabs_result_parallelized ? fabs(array[j]) : minabs_result_parallelized) ;
        }
        float minabs_result_sequential = __FLT_MAX__ ;
        for(int j = 0 ; j < data_size ; j++){
            // computing the min value sequential
             minabs_result_sequential = (fabs(array[j]) < minabs_result_sequential ? fabs(array[j]) : minabs_result_sequential) ;
        }
        free(array) ;
        if (minabs_result_parallelized != minabs_result_sequential)
            return 1 ;
    }
    // no error
    return 0 ;
}

int int_addition(int a, int b){
    return a + b ;
}
int int_substraction(int a, int b){
    return a + b ;
}

int fold_int_test(){
    int data_size = rand() % 100000 ; //max size of the rarrays : 10000
    int * array = calloc(data_size, sizeof(int)) ;
    for(int j = 0; j < data_size; j++){
        // initialization of the array
        array[j] = (rand()%100000 - (100000/2)) ;
    }
    //the test concern first the addition of all the elements of an array
    // computing parallized value
    reduction_fold_int_value.element = 0 ; reduction_fold_int_value.f = &int_addition ;
    #pragma omp parallel for reduction(fold_int : reduction_fold_int_value)
    for(int i = 0 ; i < data_size; i++){
        reduction_fold_int_value.element = reduction_fold_int_value.f(reduction_fold_int_value.element, array[i]) ;
    }
    int sequential_fold_int_value = 0 ;
    for(int i = 0; i < data_size; i++){
        sequential_fold_int_value = int_addition(sequential_fold_int_value, array[i]) ;
    }
    if (sequential_fold_int_value != reduction_fold_int_value.element)
        return 1 ;
    //the test concern second the substraction of all the elements of an array : a not commutative operation
    reduction_fold_int_value.element = 0 ; reduction_fold_int_value.f = &int_substraction ;
    #pragma omp parallel for reduction(fold_int : reduction_fold_int_value)
    for(int i = 0 ; i < data_size; i++){
        reduction_fold_int_value.element = reduction_fold_int_value.f(reduction_fold_int_value.element, array[i]) ;
    }
    sequential_fold_int_value = 0 ;
    for(int i = 0; i < data_size; i++){
        sequential_fold_int_value = int_substraction(sequential_fold_int_value, array[i]) ;
    }
    if (sequential_fold_int_value != reduction_fold_int_value.element)
        return 1 ;
    return 0 ;
}

int main(int argc, char * argv[]) {
    fprintf(stderr, "Making test ...\n") ;
    fprintf(stderr, "* ------------ *\n") ;
    fprintf(stderr, "Test of min abs int ...\n") ;
    int minabs_int_value = minabs_int_test() ;
    if (minabs_int_value != 0){
        fprintf(stderr, "Error with minabs on int\n") ;
    }
    else
        fprintf(stderr, "No problem with minabs on int ... \n") ;
    fprintf(stderr, "Test of min abs float ...\n") ;
    int minabs_float_value = minabs_float_test() ;
    if (minabs_float_value != 0){
        fprintf(stderr, "Error with minabs on float\n") ;
    }
    else
        fprintf(stderr, "No problem with minabs on float ... \n") ;

    fprintf(stderr, "Test of fold int ...\n") ;
    int fold_int_value = fold_int_test() ;
    if (fold_int_value != 0){
        fprintf(stderr, "Error with fold on int\n") ;
    }
    else
        fprintf(stderr, "No problem with fold on int ... \n") ;

    fprintf(stderr, "* ------------ *\n") ;
    fprintf(stderr, "End of the tests\n") ;
    return 0 ;
}
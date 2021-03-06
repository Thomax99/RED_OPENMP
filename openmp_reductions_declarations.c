#include <stdlib.h>

int abs_int(int x){
    return (x > 0 ? x : -x) ;
}

long abs_long(long x){
    return (x > 0 ? x : -x) ;
}

float abs_float(float x){
    return (x > 0 ? x : -x) ;
}
double abs_double(double x){
    return (x > 0 ? x : -x) ;
}

#pragma omp declare reduction(minabs : int : \
            omp_out = (abs_int(omp_in) > omp_out ? omp_out : abs_int(omp_in))) \
            initializer (omp_priv = __INT_MAX__)

#pragma omp declare reduction(minabs : float : \
            omp_out = (abs_float(omp_in) > omp_out ? omp_out : abs_float(omp_in))) \
            initializer (omp_priv = __FLT_MAX__)

#pragma omp declare reduction(minabs : long : \
            omp_out = (abs_float(omp_in) > omp_out ? omp_out : abs_long(omp_in))) \
            initializer (omp_priv = __LONG_MAX__)

#pragma omp declare reduction(minabs : double : \
            omp_out = (abs_float(omp_in) > omp_out ? omp_out : abs_double(omp_in))) \
            initializer (omp_priv = __DBL_MAX__)


typedef struct {
    int element ;
    int (*f)(int a, int b) ;
} reduction_fold_int_t ;

reduction_fold_int_t alloc_reduction_fold_int_t(reduction_fold_int_t * init){
    reduction_fold_int_t * back = malloc(sizeof(reduction_fold_int_t)) ;
    back->element = init->element ;
    back->f = init->f ;
    return *back ;
}

void apply_fold_int(reduction_fold_int_t * out, reduction_fold_int_t * in){
    out->element = out->f(out->element, in->element) ;
}

#pragma omp declare reduction(fold_int : reduction_fold_int_t : \
    apply_fold_int(&omp_out, &omp_in)) \
    initializer (omp_priv = alloc_reduction_fold_int_t(&omp_orig))


typedef struct {
    int size ;
    int * array ;
} reduction_operation_array_int_t ;

void add_array(reduction_operation_array_int_t * x, reduction_operation_array_int_t * y){
    for(int i = 0 ; i < x->size; i++){
        x->array[i]+= y->array[i] ;
    }
}

void sub_array(reduction_operation_array_int_t * x, reduction_operation_array_int_t * y){
    for(int i = 0 ; i < x->size; i++){
        x->array[i]-= y->array[i] ;
    }
}

reduction_operation_array_int_t alloc_reduction_operation_array_int_t(reduction_operation_array_int_t * initial){
    reduction_operation_array_int_t * back = malloc(sizeof(reduction_operation_array_int_t)) ;
    back->array = malloc(sizeof(int)*initial->size) ;
    for(int i = 0; i < initial->size; i++){
        back->array[i] = initial->array[i] ;
    }
    return *back ;
}

#pragma omp declare reduction(+ : reduction_operation_array_int_t : \
        add_array(&omp_out, &omp_in)) \
        initializer (omp_priv = alloc_reduction_operation_array_int_t(&omp_orig))


#pragma omp declare reduction(- : reduction_operation_array_int_t : \
        sub_array(&omp_out, &omp_in)) \
        initializer (omp_priv = alloc_reduction_operation_array_int_t(&omp_orig))


int abs_int(int x){
    return (x > 0 ? x : -x) ;
}

float abs_float(float x){
    return (x > 0 ? x : -x) ;
}

#pragma omp declare reduction(minabs : int : \
            omp_out = (abs_int(omp_in) > omp_out ? omp_out : abs_int(omp_in))) \
            initializer (omp_priv = __INT_MAX__)

#pragma omp declare reduction(minabs : float : \
            omp_out = (abs_float(omp_in) > omp_out ? omp_out : abs_float(omp_in))) \
            initializer (omp_priv = __FLT_MAX__)


typedef struct {
    int element ;
    int (*f)(int a, int b) ;
} reduction_fold_int_t ;

reduction_fold_int_t reduction_fold_int_value ;

#pragma omp declare reduction(fold_int : reduction_fold_int_t : \
    omp_out.element = omp_out.f(omp_out.element, omp_in.element)) \
    initializer (omp_priv = omp_orig)

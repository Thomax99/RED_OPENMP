# OPENMP REDUCTION FRAMEWORK

This repository present a simple way to make parallelism with openmp when using loops which needs several particular variables. The framework implements differents maneer of reduction for transitive operations with classical types or non-classical types

The C file openmp_reductions_tests.c give some example of the usage of this reductions

## Operations on classical types

- minabs : choose the minimal absolute value on numerical types for long, int, double, float)

Example :

```C
int minabs_result_parallelized = __INT_MAX__ ; // initalization at the max because we search to be the min ...
        #pragma omp parallel for reduction(minabs: minabs_result_parallelized)
        for(int j = 0 ; j < data_size ; j++){
            // computing the min value parallized
             minabs_result_parallelized = (abs(array[j]) < minabs_result_parallelized ? abs(array[j]) : minabs_result_parallelized) ;
        }
```

- fold : given a function f and an element of a given numerical type el, compute f(...(f(el, et1), et2)..., etn) ; when eti is the element computed by the thread of number i

    To make this, we make a typedef struct named reduction_fold_{type}_t which encapsulate an element and a function. The element given could be the neutral element of the function. The function take two arguments of the same type and has to return one element of the same type than the two arguments. To make the function run correctly, you just have to create a reduction_fold_{type}_t element in which you encapsulate the work and to give this to you operation of reduction.

Example :

```C
int int_addition(int a, int b){
    return a + b ;
}

...

reduction_fold_int_t reduction_fold_int_value ;
    reduction_fold_int_value.element = 0 ; reduction_fold_int_value.f = &int_addition ;
    #pragma omp parallel for reduction(fold_int : reduction_fold_int_value)
    for(int i = 0 ; i < data_size; i++){
        reduction_fold_int_value.element = reduction_fold_int_value.f(reduction_fold_int_value.element, array[i]) ;
    }
...
```

## Operations on array

We've wanted to implement the classical operators on arrays. This means +, -, *, &, |, ^, && and ||. This also means that the operators are made on the array, ie + for int array means for each i < sizeof(array), result[i] = array1[i] + array2[i]. See tests for more example.

To make this correctly, we've make a typedef struct named reduction_operation_array_{type}_t, which encapsulate the array and the size of the array. Declare a variable of this type correctly initialized to use the operations well.

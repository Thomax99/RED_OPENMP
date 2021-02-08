# OPENMP REDUCTION FRAMEWORK

This repository present a simple way to make parallelism with openmp when using loops which needs several particular variables. The framework implements differents maneer of reduction for transitive operations with classical types or non-classical types

The C file openmp_reductions_tests.c give some example of the usage of this reductions

## Operations on classical types

- minabs : choose the min absolute value on numerical types
- fold : given a function f and an element of a given numerical type el, compute f(...(f(el, et1), et2)..., etn) ; when eti is the element computed by the thread of number i

    To make this, we make a typedef struct named reduction_fold_{type}_t which encapsulate an element and a function. The element given could be the neutral element of the function. The function take two arguments of the same type and has to return one element of the same type than the two arguments. To make the function run correctly, you just have to make just before the call a redefinition of reduction_fold_{type}_value, which is the reduction_fold_t element used to do all the work.
    
#include <stdio.h>

int main()
{
  #pragma omp parallel num_threads(100)
  {
    printf("Bonjour !\n");
    printf("Au revoir !\n");
  }

  return 0;
}

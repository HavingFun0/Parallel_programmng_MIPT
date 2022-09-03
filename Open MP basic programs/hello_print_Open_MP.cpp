#include <stdio.h>
#include <omp.h>

int main() {
	printf("%d \n", omp_get_num_procs());	
#pragma omp parallel num_threads(6)
	{
		printf("Hello OMP! I'm the  %d of %d \n",omp_get_thread_num(), omp_get_num_threads());
	}
	printf("\n Seq part 2. Number -- %d, size -- %d \n",omp_get_thread_num(), omp_get_num_threads());
	return 0;
}

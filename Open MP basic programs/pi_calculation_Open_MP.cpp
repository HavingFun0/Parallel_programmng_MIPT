#include <iostream>
#include <cmath>
#include <stdio.h>
#include <omp.h>

using namespace std;

int main(int argc, char* argv[]) {
	int n=100000;
	double s=0, x=0;
	
/*
	int size;
	size = omp_get_num_procs();
#pragma omp parallel reduction(+:s)
	{
		double x = 0;
		int from_to [2];
		int i = omp_get_thread_num();
		int size = omp_get_num_threads();
			int from = i * n / size;
			int to = (i+1) * n / size;
			from_to[0] = from;
			from_to[1] = to;
			if ( i == size - 1) {
				from_to[0] = i*n/size;
				from_to[1] = n;
			}
		x = 2.0/n *  from_to[0];
		for (int j = from_to[0]; j < from_to[1]; j++) {
			s += 1.0/n * (sqrt(4-x*x) + sqrt(4-(x + 2.0/n )*(x+2.0/n)));  
			x += 2.0/n;
		}
	}
*/
#pragma omp parallel reduction(+:s)
{
	#pragma omp for
	for (int  j = 0 ; j < n; j++) {
			s += 1.0/n * (sqrt(4-j * 2.0/n* j * 2.0/n) + sqrt(4-(j*2.0/n + 2.0/n )*(j*2.0/n+2.0/n)));  
	}
}
	printf("%lf \n",s);
	
	return 0;
}

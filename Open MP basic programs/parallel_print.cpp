#include <stdio.h>
#include <omp.h>

int x0 = 2;

int main(){
	printf("Sequential part 1\n");
	int x1=2;

	#pragma omp parallel private(x0, x1) 
	{
		int x2 = 0; static int x3 = 0;
		x0++; x1++; x2++; x3++;
		printf("Par x0=%d x1=%d x2=%d x3=%d\n", x0,x1,x2,x3);
	}
	printf("Seq x0=%d x1=%d \n", x0,x1);
	return 0;
}

#include <mpi.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>

void check_MPI(int rc) {
	if (rc != MPI_SUCCESS) {
		std::cout << "ERROR" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
}

int main(int argc, char* argv[]) {
	check_MPI(MPI_Init(& argc, & argv));
	int size, rank;
	check_MPI(MPI_Comm_size(MPI_COMM_WORLD, &size));
	check_MPI(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
	srandom(rank+1);
	int* times, *displ;
	double* res, *arr;
		
	int times_o = random()%7 +1;
	
	double numb = sqrt(rank+1.0);
	
	arr = new double [times_o];
	for (int i =0; i < times_o; i++) {
		arr[i] = numb;
	}	
	if (rank == 0) {
		times = new int[size];
		displ = new int[size];	
	}
	
	check_MPI(MPI_Gather(& times_o, 1, MPI_INT, times, 1, MPI_INT, 0, MPI_COMM_WORLD));
	
	int size_of_res;
	if (rank == 0) {
		int tmp_sum=0;
		for (int i=0; i < size; i++) {
			tmp_sum += times[i];
		}
		size_of_res = tmp_sum;
		displ[0] = 0;
		res = new double [size_of_res];
		for (int i=1; i < size; i++) {
			displ[i] = displ[i-1] + times[i-1];
		}
	}	
	check_MPI(MPI_Gatherv(arr, times_o, MPI_DOUBLE, res, times, displ, MPI_DOUBLE, 0, MPI_COMM_WORLD));

	if (rank==0) {

		FILE *ff;
		
		if ((ff = fopen("sfile", "w+")) == NULL){
           		delete[] arr;
           		delete [] times;
            		delete [] displ;
            		delete [] res;
            		MPI_Abort(MPI_COMM_WORLD, 2);
        	}
		int j=0, jend, i;
        	for(int i = 0; i < size; i++){
            		jend = displ[i]+times[i];
            		for(; j < jend ; j++) {
             		 	fprintf(ff, "%4.3f ", res[j]);
			}
           		fprintf(ff,"\n");
        	}

     		fclose(ff);
		delete [] times;
		delete [] displ;
		delete [] res;
	}
		
	delete [] arr;
	MPI_Finalize();
	return 0;
} 

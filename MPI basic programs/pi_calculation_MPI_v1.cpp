#include <iostream>
#include <cmath>
#include <stdio.h>
#include <mpi.h>

void check_MPI(int rc) {
        if (rc != MPI_SUCCESS) {
                std::cout << "ERROR" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
        }
}

using namespace std;

int main(int argc, char* argv[]) {
	check_MPI(MPI_Init(& argc, & argv));
	int n;
	FILE *f;
	f = fopen("n_for_pi.txt", "r");
	fscanf(f, "%d", &n);
	fclose(f);
	double s=0, x=0;
	int size, rank, acc_rk;

	check_MPI(MPI_Comm_size(MPI_COMM_WORLD, &size));
        check_MPI(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
	MPI_Status mpi_status;

	int from_to [2];
	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			int from = i * n / size;
			int to = (i+1) * n / size;
			from_to[0] = from;
			from_to[1] = to;
			if ( i == size - 1) {
				from_to[0] = i*n/size;
				from_to[1] = n;
			}
			check_MPI(MPI_Send(&from_to, 2, MPI_INT, i, 0, MPI_COMM_WORLD));
		}	
	} else {
		check_MPI(MPI_Recv(&from_to, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,  &mpi_status));
		x = 2.0/n *  from_to[0];
		for (int i = from_to[0]; i < from_to[1]; i++) {
			s += 1.0/n * (sqrt(4-x*x) + sqrt(4-(x + 2.0/n )*(x+2.0/n)));  
			x += 2.0/n;
		}
		check_MPI(MPI_Send(&s, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD));
	}

	if (rank == 0) {
		for (int i = 0; i < n/size; i++) {
			s += 1.0/n * (sqrt(4-x*x) + sqrt(4-(x + 2.0/n )*(x+2.0/n)));  
			x += 2.0/n;
		}
		for (int i = 1; i < size; i ++) {
			double tmp;
			check_MPI(MPI_Recv(&tmp, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD,  &mpi_status));
			s += tmp;
		}
		printf("%lf \n",s);
	}
	MPI_Finalize();
	return 0;
}

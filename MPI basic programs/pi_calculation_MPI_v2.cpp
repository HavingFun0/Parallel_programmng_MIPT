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
	double s=0, x=0;
	int size, rank, acc_rk;
	int N;
	int from_to [2];
	check_MPI(MPI_Init(& argc, & argv));
	check_MPI(MPI_Comm_size(MPI_COMM_WORLD, &size));
        check_MPI(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
	MPI_Status mpi_status;
	double fin_sum=0;
	int n;
	if (rank == 0) {	
		FILE *f;
		f = fopen("n_for_pi.txt", "r");
		fscanf(f, "%d", &n);
		fclose(f);
	}

	check_MPI(MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD));
	
	from_to[0] = n/size*(rank-1);
	from_to[1] = n/size*(rank);
	if (rank == size -1) {
		from_to[1] = n;
	}

	x = 2.0/n * from_to[0];
	if (from_to[1] != 0) {
		for (int i = from_to[0]; i < from_to[1]; i++) {
			s += 1.0/n * (sqrt(4-x*x) + sqrt(4-(x + 2.0/n )*(x+2.0/n)));  
			x += 2.0/n;
		}
	}

	check_MPI(MPI_Reduce(rank == 0 ? MPI_IN_PLACE: &s, &s,1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD));

	if (rank == 0) {
		printf("%lf \n",s);
	}
	MPI_Finalize();
	return 0;
}

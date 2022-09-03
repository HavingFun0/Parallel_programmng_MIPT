#include <iostream>
#include <stdio.h>
#include <mpi.h>

void check_MPI(int rc) {
        if (rc != MPI_SUCCESS) {
                std::cout << "ERROR" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
        }
}

int main(int argc, char* argv[]) {
        using namespace std;
        check_MPI(MPI_Init(& argc, & argv));
        int size, rank, acc_rk;
	int a;
	MPI_Status mpi_status;
        check_MPI(MPI_Comm_size(MPI_COMM_WORLD, &size));
        check_MPI(MPI_Comm_rank(MPI_COMM_WORLD, &rank));

        if (rank != 0) {
                check_MPI(MPI_Recv(&a, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD,  &mpi_status));
		a += rank;
		cout << "I'm process number " << rank << ". I get number " << a - rank << endl;
        } else {
		a = 0;
	}	

	if (rank+1 == size) { 
		check_MPI(MPI_Send(&a, 1, MPI_INT, 0, rank, MPI_COMM_WORLD));
		cout << "I am process number "<< rank << " and I sent " << a << " to " << (rank+1)%size << "." << endl;
	} else {
		
		check_MPI(MPI_Send(&a, 1, MPI_INT, rank+1, rank, MPI_COMM_WORLD));
			
		cout <<  "I am process number "<< rank << " and I sent "  << a << " to " << (rank+1)%size << "." << endl;
	}

	if (rank == 0) {
           	check_MPI(MPI_Recv(&a, 1, MPI_INT, -1%size, MPI_ANY_TAG, MPI_COMM_WORLD,  &mpi_status));
		cout << "I'm process number " << 0 << ". I get number " << a - rank << endl;
	}
        MPI_Finalize();
        return 0;
}


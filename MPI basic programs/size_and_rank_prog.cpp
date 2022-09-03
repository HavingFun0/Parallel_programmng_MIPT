#include <mpi.h>
#include <iostream>
#include <stdio.h>

int main(int argc, char* argv[]) {
	using namespace std;
	
	int rc, size, rank;
	rc = MPI_Init(& argc, & argv);
	if (rc != MPI_SUCCESS) {
		cout << "ERROR" << endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	} 
	cout << "Hello world!" << endl;
	
	cout << "--------------------------------" << endl;
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rc != MPI_SUCCESS) {
		cout << "ERROR" << endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}	
	cout << "I'm prosses number " << rank << endl;
	rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rc != MPI_SUCCESS) {
		cout << "ERROR" << endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}	
	cout << "There are " << size << " processors" << endl;

	cout << "--------------------------------" << endl;
	MPI_Finalize();
	return 0;
}

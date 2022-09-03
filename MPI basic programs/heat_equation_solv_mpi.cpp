#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <iostream>

void check_MPI(int rc) {
	if (rc != MPI_SUCCESS) {
		std::cout << "ERROR" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
}

int main(int argc, char *argv[]){
    
    check_MPI(MPI_Init(&argc, & argv));
    double *u, *unew, delta, maxdelta;
    double eps=1.e-6;
    double h, tau;
    
	int rank, size;
	check_MPI(MPI_Comm_size(MPI_COMM_WORLD, &size));
	check_MPI(MPI_Comm_rank(MPI_COMM_WORLD, &rank));  
    
	
	int N;
    int i;
    
	MPI_Status mpi_status;

    FILE *ff;
    
    if(argc !=2 ){
	printf("Usage: exefile npoints\n");
	exit(-1);
    }
    
    N = atoi(argv[1]);
    if (N == 0) {
	printf("Set N to 1000\n");
	N =1000;
    } else {
	printf("Set N to %d\n", N);
    }
    
    if((u =(double*) malloc((N+1)*sizeof(double))) == NULL){
	printf("Can't allocate memory for u\n");
	exit(-1);
    }

    if((unew =(double*) malloc((N+1)*sizeof(double))) == NULL){
	printf("Can't allocate memory for unew\n");
	free(u);
	exit(-1);
    }
    
    // begin & bound values
    
    for(i=0; i<N+1; i++){
		u[i] = 0;
    }
    
    unew[0] = u[0] = 1.0;
    unew[N] = 0;
    
    h = 1.0/N;
    tau = 0.5*(h*h);
        
	int from_to[2];
	
	from_to[0] = (N)/size * (rank);
	from_to[1] = (N)/size * (rank+1);
	if (rank == size-1) {
		from_to[1] = N;
	}	
		
	if (rank == 0) from_to[0] =1;

	
    while(1){
		for (i=from_to[0]; i<from_to[1]; i++){
		 unew[i] = u[i]+(tau/(h*h))*(u[i-1]-2*u[i]+u[i+1]);
		}   	
	
        maxdelta = 0;//мб они не считают граничные
        for (i=from_to[0]; i<from_to[1]; i++){
            delta = fabs(unew[i]-u[i]);
            if(delta > maxdelta) maxdelta = delta;
        }

	check_MPI(MPI_Reduce(rank==0 ? MPI_IN_PLACE: &maxdelta, &maxdelta, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD));
	check_MPI(MPI_Bcast(&maxdelta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD));
        
	if(maxdelta < eps) break;
	
        for(i = from_to[0]; i < from_to[1]; i++){
    	    u[i] = unew[i];;
        }

        //send right border
	if (rank!= size -1) {
		check_MPI(MPI_Send(&u[from_to[1]-1], 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD));
	}
	if (rank != 0) {
		check_MPI(MPI_Recv(&u[from_to[0]-1],1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &mpi_status ));
	}


	//send left border
	if (rank!= 0) {
		check_MPI(MPI_Send(&u[from_to[0]], 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD));
	}
	if (rank != size-1) {
		check_MPI(MPI_Recv(&u[from_to[1]],1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &mpi_status));
	}

    }
	
    int *count, *displ;
    if (rank ==0) {
		count = new int[size];
		displ = new int[size];
    }	
    int times = from_to[1] - from_to[0] ;
    check_MPI(MPI_Gather(&times, 1, MPI_INT, count, 1, MPI_INT, 0, MPI_COMM_WORLD));

    if (rank == 0) {

	displ[0] = from_to[0];
	for (int aa=1; aa < size; aa++) {
		displ[aa] = displ[aa-1] + count[aa-1];
	}
    }
    check_MPI(MPI_Gatherv(unew+from_to[0], times, MPI_DOUBLE, unew, count, displ, MPI_DOUBLE, 0, MPI_COMM_WORLD));

    if (rank == 0) {
    
	    if ((ff = fopen("resfile_check", "w+")) == NULL){
 	      printf("Can't open file\n"); free(u); free(unew);
    	      exit(-1);
   	    }
    
    	    for(i=0; i<N+1; i++)
            fprintf(ff, "%f ", unew[i]);
			
    	    fclose(ff); 
	    delete[] count;
	    delete[] displ;
    }
    free(u); free(unew);
    MPI_Finalize();

    return 0;  
}      

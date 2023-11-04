// Activity 1 - Question 1

#include <mpi.h>
#include <stdio.h>
#include <cstring>

int main(int argc, char** argv) {
    int numtasks, rank, name_len, tag=1; 
    char name[MPI_MAX_PROCESSOR_NAME];
    char msgOut[30]= "hello";
    char msgIn[30];

    MPI_Status status;
    // Initialize the MPI environment
    MPI_Init(&argc,&argv);
    // Get the number of tasks/process
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    // Get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Find the processor name
    MPI_Get_processor_name(name, &name_len);

    if(rank == 0){
        // Print off a hello world message
        for (size_t i = 1; i < numtasks; i++)
        {

            MPI_Send(&msgOut, std::strlen(msgOut), MPI_CHAR, i, tag, MPI_COMM_WORLD);

        }
    }else{
        // Print off a hello world message
        MPI_Recv(&msgIn, 30, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        printf("Process %d : %s\n", rank, msgIn);
    }
    // Finalize the MPI environment
    MPI_Finalize();
}

// Activity 1 - Question 2

// #include <mpi.h>
// #include <stdio.h>
// #include <cstring>

// int main(int argc, char** argv) {
//     int numtasks, rank, name_len, tag=1; 
//     char name[MPI_MAX_PROCESSOR_NAME];
//     char msg[30];
//     MPI_Status status;
//     // Initialize the MPI environment
//     MPI_Init(&argc,&argv);

//     // Get the number of tasks/process
//     MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

//     // Get the rank
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//     // Find the processor name
//     MPI_Get_processor_name(name, &name_len);


//     if(rank == 0){
//         // Print off a hello world message
//         strcpy(msg, "Hello from Master");
//         printf("Brodcast the %s\n", msg);
//     }

//     MPI_Bcast(&msg, 30, MPI_CHAR, 0, MPI_COMM_WORLD);


//      if(rank != 0){
//         // Print off a hello world message
//         // MPI_Recv(&msgIn, 30, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
//         printf("Process %d got this message: %s\n", rank, msg);
//     }

//     // Finalize the MPI environment
//     MPI_Finalize();
// }
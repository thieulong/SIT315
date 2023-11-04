#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

// Define matrix dimension (NxN)
const int N = 200;

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(static_cast<unsigned int>(time(0)));

    // Initialize matrices A and B with random values
    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = static_cast<double>(rand() % 10);
            B[i][j] = static_cast<double>(rand() % 10);
        }
    }

    // Broadcast matrices A and B to all processes
    for (int i = 0; i < N; ++i) {
        MPI_Bcast(A[i].data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(B[i].data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Initialize the result matrix C
    std::vector<std::vector<double>> C(N, std::vector<double>(N));

    // Perform matrix multiplication
    double start_time = MPI_Wtime(); 

    // Perform matrix multiplication
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double end_time = MPI_Wtime(); 

    // Convert execution time from seconds to microseconds
    double execution_time_microseconds = (end_time - start_time) * 1000000;

    if (rank == 0) {
        std::cout << "Execution Time: " << execution_time_microseconds << " microseconds" << std::endl;
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <omp.h>
#include <chrono> // Include chrono for timing

// Define matrix dimension (NxN)
const int N = 200;

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Initialize matrices A and B
    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));

    // Initialize matrices A and B with random values using OpenMP parallelization
    #pragma omp parallel for collapse(2)
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

    // Perform matrix multiplication with OpenMP parallelization
    auto start_time = std::chrono::high_resolution_clock::now(); // Start the timer

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now(); // Stop the timer
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Print the execution time in microseconds
    if (rank == 0) {
        std::cout << "Execution Time: " << duration.count() << " microseconds" << std::endl;
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}

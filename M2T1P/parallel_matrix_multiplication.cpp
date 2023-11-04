#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip> 
#include <pthread.h> 

using namespace std;
using namespace std::chrono;

const int N = 200; 
const int numberOfThreads = 8;

int A[N][N];
int B[N][N];
int C[N][N];

void* generateRandomMatrix(void* specificThread) {
    int isolate = N / numberOfThreads;
    int startRow = static_cast<int>(reinterpret_cast<long>(specificThread)) * isolate;
    int endRow = startRow + isolate;

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10; 
            B[i][j] = rand() % 10;
        }
    }

    pthread_exit(NULL);
}

void* matrixMultiplicationChunk(void* specificThread) {
    int isolate = N / numberOfThreads;
    int startRow = static_cast<int>(reinterpret_cast<long>(specificThread)) * isolate;
    int endRow = startRow + isolate;

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[numberOfThreads];
    int specificThreads[numberOfThreads];

    // auto startGen = high_resolution_clock::now();

    for (int i = 0; i < numberOfThreads; ++i) {
        specificThreads[i] = i;
        int result = pthread_create(&threads[i], NULL, generateRandomMatrix, reinterpret_cast<void*>(specificThreads[i]));
    }

    for (int i = 0; i < numberOfThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // auto endGen = high_resolution_clock::now();
    // auto genDuration = duration_cast<microseconds>(endGen - startGen);

    // cout << fixed << setprecision(3) << "Matrix generation time: " << genDuration.count() / 1000.0 << " ms" << endl;

    auto startMultiply = high_resolution_clock::now();

    for (int i = 0; i < numberOfThreads; ++i) {
        specificThreads[i] = i;
        int result = pthread_create(&threads[i], NULL, matrixMultiplicationChunk, reinterpret_cast<void*>(specificThreads[i]));
    }

    for (int i = 0; i < numberOfThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    auto endMultiply = high_resolution_clock::now();
    auto multiplyDuration = duration_cast<microseconds>(endMultiply - startMultiply);

    ofstream outputFile("pthread_matrix_multiplication_result.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outputFile << C[i][j] << " ";
            }
            outputFile << endl;
        }
        outputFile.close();
    }

    cout << fixed << setprecision(3) << "Parallel with pthread execution time: " << multiplyDuration.count() / 1000.0 << " ms" << endl;

    return 0;
}

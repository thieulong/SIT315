#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

void merge(int arr[], int left[], int right[], int leftSize, int rightSize) {
    int i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i] < right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = left[i++];
    }

    while (j < rightSize) {
        arr[k++] = right[j++];
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 1000000;
    int* arr = new int[n];
    int* localArr = nullptr;
    int localSize = n / size;

    srand(time(0) + rank * 1000);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    localArr = new int[localSize];
    MPI_Scatter(arr, localSize, MPI_INT, localArr, localSize, MPI_INT, 0, MPI_COMM_WORLD);

    auto start = high_resolution_clock::now(); 

    quickSort(localArr, 0, localSize - 1);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    MPI_Barrier(MPI_COMM_WORLD);

    int* sortedArr = nullptr;
    if (rank == 0) {
        sortedArr = new int[n];
    }

    MPI_Gather(localArr, localSize, MPI_INT, sortedArr, localSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        quickSort(sortedArr, 0, n - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Execution time: " << duration.count() << " milliseconds" << endl;
    }

    delete[] arr;
    delete[] localArr;
    if (rank == 0) {
        delete[] sortedArr;
    }

    MPI_Finalize();
    return 0;
}

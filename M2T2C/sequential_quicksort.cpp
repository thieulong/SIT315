#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
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

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int n = 1000000;
    int* arr = new int[n];

    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    //cout << "Original list: ";
    //printArray(arr, n);

    auto start = high_resolution_clock::now(); 

    quickSort(arr, 0, n - 1);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start); 

    //cout << "Sorted list with Quicksort: ";
    //printArray(arr, n);

    cout << "Execution time: " << duration.count() << " ms" << endl;

    delete[] arr;
    return 0;
}

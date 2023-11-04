__kernel void quicksort(__global int* array, const int left, const int right) {
    int l = left;
    int r = right;
    int stack[32]; // Adjust the stack size as needed
    int top = -1;

    stack[++top] = l;
    stack[++top] = r;

    while (top >= 0) {
        r = stack[top--];
        l = stack[top--];

        int pivotIndex = l;
        int pivotValue = array[r];
        int i = l - 1;

        for (int j = l; j < r; j++) {
            if (array[j] < pivotValue) {
                i++;
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }

        int temp = array[i + 1];
        array[i + 1] = array[r];
        array[r] = temp;

        pivotIndex = i + 1;

        if (pivotIndex - 1 > l) {
            stack[++top] = l;
            stack[++top] = pivotIndex - 1;
        }

        if (pivotIndex + 1 < r) {
            stack[++top] = pivotIndex + 1;
            stack[++top] = r;
        }
    }
}

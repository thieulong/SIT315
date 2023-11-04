#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <sys/time.h>

#define PRINT 0

size_t SZ = 1000000;
int *arr;
cl_mem bufArr;

cl_device_id device_id;
cl_context context;
cl_program program;
cl_kernel kernel;
cl_command_queue queue;
cl_event event = NULL;

int err;

cl_device_id create_device();
void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname);
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename);
void setup_kernel_memory();
void copy_kernel_args();
void free_memory();

void init(int *&A, int size);
void print(int *A, int size);

int main(int argc, char **argv) {
    struct timeval start, end;

    init(arr, SZ);

    // Initialize OpenCL
    setup_openCL_device_context_queue_kernel((char *)"./quicksort_opencl.cl", (char *)"quicksort");

    setup_kernel_memory();
    copy_kernel_args();

    gettimeofday(&start, NULL);

    // Submit the kernel for execution
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &SZ, NULL, 0, NULL, &event);
    clWaitForEvents(1, &event);

    gettimeofday(&end, NULL);

    // Copying data from the device back to host array
    clEnqueueReadBuffer(queue, bufArr, CL_TRUE, 0, SZ * sizeof(int), &arr[0], 0, NULL, NULL);

    // Result array
    print(arr, SZ);

    // Calculate and print execution time in milliseconds
    double elapsed = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Execution Time: %.2f milliseconds\n", elapsed);

    // Frees memory for device, kernel, queue, etc.
    free_memory();

    return 0;
}

void init(int *&A, int size) {
    A = (int *)malloc(sizeof(int) * size);

    for (long i = 0; i < size; i++) {
        A[i] = rand() % 1000; 
    }
}

void print(int *A, int size) {
    if (PRINT == 0) {
        return;
    }

    for (long i = 0; i < size; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

void free_memory() {
    // Free the buffers
    clReleaseMemObject(bufArr);

    // Free OpenCL objects
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);

    free(arr);
}

void copy_kernel_args() {
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufArr);

    if (err < 0) {
        perror("Couldn't create a kernel argument");
        printf("error = %d", err);
        exit(1);
    }
}

void setup_kernel_memory() {
    bufArr = clCreateBuffer(context, CL_MEM_READ_WRITE, SZ * sizeof(int), NULL, NULL);

    // Copy array to the GPU
    clEnqueueWriteBuffer(queue, bufArr, CL_TRUE, 0, SZ * sizeof(int), &arr[0], 0, NULL, NULL);
}

void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname) {
    device_id = create_device();
    cl_int err;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err < 0) {
        perror("Couldn't create a context");
        exit(1);
    }

    program = build_program(context, device_id, filename);
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
    if (err < 0) {
        perror("Couldn't create a command queue");
        exit(1);
    }

    kernel = clCreateKernel(program, kernelname, &err);
    if (err < 0) {
        perror("Couldn't create a kernel");
        printf("error =%d", err);
        exit(1);
    }
}

cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename) {
    cl_program program;
    FILE *program_handle;
    char *program_buffer, *program_log;
    size_t program_size, log_size;

    /* Read program file and place content into buffer */
    program_handle = fopen(filename, "r");
    if (program_handle == NULL) {
        perror("Couldn't find the program file");
        exit(1);
    }
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char *)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);

    /* Create program from file */
    program = clCreateProgramWithSource(ctx, 1, (const char **)&program_buffer, &program_size, &err);
    if (err < 0) {
        perror("Couldn't create the program");
        exit(1);
    }
    free(program_buffer);

    /* Build program */
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err < 0) {
        /* Find size of log and print to std output */
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        program_log = (char *)malloc(log_size + 1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log, NULL);
        printf("%s\n", program_log);
        free(program_log);
        exit(1);
    }

    return program;
}

cl_device_id create_device()
{

   cl_platform_id platform;
   cl_device_id dev;
   int err;

   /* Identify a platform */
   err = clGetPlatformIDs(1, &platform, NULL);
   if (err < 0)
   {
      perror("Couldn't identify a platform");
      exit(1);
   }

   // Access a device
   // GPU
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if (err == CL_DEVICE_NOT_FOUND)
   {
      // CPU
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if (err < 0)
   {
      perror("Couldn't access any devices");
      exit(1);
   }

   return dev;
}

/*
Please put these code below in a file named quicksort_opencl.cl in order to execute this MPI + OpenCL program

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
*/
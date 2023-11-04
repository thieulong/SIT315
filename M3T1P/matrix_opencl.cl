__kernel void square_magnitude(const int size,
                      __global int* v) {
    
    // Thread identifiers
    const int globalIndex = get_global_id(0); // Row ID of C (0..M)   
 
    //uncomment to see the index each PE works on
    //printf("Kernel process index :(%d)\n ", globalIndex);

    v[globalIndex] = v[globalIndex] * v[globalIndex];
}

__kernel void multiply_matrix(const int size,
                      const __global int* v1,const __global int* v2,__global int* v3) {
    
    // Thread identifiers
    const int i = get_global_id(0);    
    const int j = get_global_id(1);    

    const int index = (i * size) + j;
 
    //uncomment to see the index each PE works on
    //printf("Kernel process index :(%d,%d) and 1d index is %d\n", i, j, index);

    v3[index] = v2[index] * v1[index];
}

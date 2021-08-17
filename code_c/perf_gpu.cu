/* Teste la peformance de rsqrt sur un grand nombre de valeurs aléatoires (version GPU)
 * À compiler avec `nvcc perf_gpu.cu -o test -O3` (requière CUDA!)
 */


#include <cmath>
#include <chrono>
#include <iostream>
#include <cuda.h>

#define N_FLOAT 100000000
#define MAX_FLOAT 1000

__global__ void rsqrt_vec(float* vec_source, int n)
{
    int i = blockIdx.x*blockDim.x + threadIdx.x;
    if (i < n) // évite les débordements
        vec_source[i] = rsqrtf(vec_source[i]);
}

int main() {
    float *floats_source = NULL, *floats_dest = NULL;
    float *d_comm = NULL;
    int i;
    
    cudaSetDevice(0);
    srand((unsigned) time(NULL));
    
    // créee le tableau de flottants
    floats_source = (float*) malloc(N_FLOAT *sizeof(float));
    
    if(floats_source == NULL) {
        printf("error allocating floats_source");
        return -1;
    }
    
    for(i=0; i < N_FLOAT; i++) // génère N_FLOAT float entre 1 et MAX_FLOAT
        floats_source[i] = ((float) (1 + rand())) / RAND_MAX * MAX_FLOAT;
    
    // alloue de la mémoire pour le résultat
    floats_dest = (float*) malloc(N_FLOAT *sizeof(float));
    
    if(floats_dest == NULL) {
        printf("error allocating floats_dest");
        return -1;
    }
    
    // crée la mémoire sur le GPU
    cudaMalloc(&d_comm, N_FLOAT *sizeof(float));
    
    // teste les performances:
    auto before = std::chrono::high_resolution_clock::now();
    cudaMemcpy(d_comm, floats_source, N_FLOAT *sizeof(float), cudaMemcpyHostToDevice);
    
    int blocksize = 512;
    int nblock = N_FLOAT/blocksize + (N_FLOAT % blocksize > 0 ? 1: 0);
    rsqrt_vec<<<nblock, blocksize>>>(d_comm, N_FLOAT);
    
    cudaMemcpy(floats_dest, d_comm, N_FLOAT *sizeof(float), cudaMemcpyDeviceToHost);
    auto t_rsqrt = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - before);
    
    std::cout << "Le tout a prit " 
              << ((double) t_rsqrt.count() / 1e6)
              << "ms, donc rsqrt: " 
              << ((double) t_rsqrt.count() / N_FLOAT) 
              << " ns/floats" << std::endl;
    
    free(floats_source);
    free(floats_dest);
    cudaFree(d_comm);
}

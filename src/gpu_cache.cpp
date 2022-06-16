#include <gpu_cache.hpp>
#include <nv_gpu_cache.hpp>
#include <iostream>


GPUCache::GPUCache(const size_t set_size, const size_t set_associativity, const size_t slab_size, const size_t embedding_vec_size)
            :CacheBase(set_size,set_associativity,slab_size,embedding_vec_size){
    
    cache = new Cache_(set_size, embedding_vec_size);
    // allocate the GPUCache array, etc.            
}


void GPUCache::Query(const SparseInput& in_keys, D_type* out_data){
    CUDA_CHECK(cudaSetDevice(0));
    const size_t query_length = in_keys.indices.size();
    const size_t embedding_vec_size = get_emb_dim();

    // Host buffers
    K_type* h_query_keys = new K_type[query_length];
    D_type* h_vals_retrieved = new D_type[query_length * embedding_vec_size];
    K_type* h_missing_keys = new K_type[query_length];
    uint64_t* h_missing_index = new uint64_t[query_length];
    D_type* h_missing_vals = new D_type[query_length * embedding_vec_size];

    size_t h_missing_len;

    // GPU buffers
    K_type* d_query_keys; // Buffer holding keys to be queried
    uint64_t* d_missing_index; // Buffers holding missing index from query
    K_type* d_missing_keys; // Buffer holding missing keys from query
    size_t* d_missing_len; // missing length
    D_type* d_vals_retrieved; // Buffer holding values retrieved from query
    D_type* d_missing_vals; // Buffer holding values for missing keys

    const size_t task_per_warp_tile = 1;
    cudaStream_t stream;
    CUDA_CHECK(cudaStreamCreate(&stream));

    // Allocate device side buffers
    CUDA_CHECK(cudaMalloc((void**)&d_query_keys, query_length * sizeof(K_type)));
    CUDA_CHECK(cudaMalloc((void**)&d_vals_retrieved, query_length * embedding_vec_size * sizeof(D_type)));
    CUDA_CHECK(cudaMalloc((void**)&d_missing_keys, query_length * sizeof(K_type)));
    CUDA_CHECK(cudaMalloc((void**)&d_missing_vals, query_length * embedding_vec_size * sizeof(D_type)));
    CUDA_CHECK(cudaMalloc((void**)&d_missing_index, query_length * sizeof(uint64_t)));
    CUDA_CHECK(cudaMalloc((void**)&d_missing_len, sizeof(size_t)));


    // Copy keys to gpu
    CUDA_CHECK(cudaMemcpyAsync(d_query_keys, h_query_keys, query_length * sizeof(K_type), cudaMemcpyHostToDevice, stream));

    cache->Query(d_query_keys, query_length, d_vals_retrieved, d_missing_index, d_missing_keys,  d_missing_len, stream);
    
    // Wait for finishing
    CUDA_CHECK(cudaStreamSynchronize(stream));

    // Copy the data back to host
    CUDA_CHECK(cudaMemcpyAsync(h_vals_retrieved, d_vals_retrieved, query_length * embedding_vec_size * sizeof(float), cudaMemcpyDeviceToHost, stream));
    CUDA_CHECK(cudaMemcpyAsync(h_missing_index, d_missing_index, query_length * sizeof(uint64_t), cudaMemcpyDeviceToHost, stream));
    CUDA_CHECK(cudaMemcpyAsync(h_missing_keys, d_missing_keys, query_length * sizeof(K_type), cudaMemcpyDeviceToHost, stream));
    CUDA_CHECK(cudaMemcpyAsync(&h_missing_len, d_missing_len, sizeof(size_t), cudaMemcpyDeviceToHost, stream));
    CUDA_CHECK(cudaStreamSynchronize(stream));
    printf("gpu_cache: Missing key: %zu. Hit rate: %f %%.\n", h_missing_len, 100.0f - (((float)h_missing_len / (float)query_length) * 100.0f));

    // Deal with missing keys
    CacheBase * next_level_cache = get_next_level_cache();

    if(!next_level_cache){
        std::cerr<<"NO Next Level Cache!"<<std::endl;
    }

    SparseInput missing_keys;
    for(size_t missing_idx = 0; missing_idx < h_missing_len; missing_idx++){
        missing_keys.indices.push_back(h_missing_keys[missing_idx]);
    }

    // Query from the next level cache
    next_level_cache->Query(missing_keys, h_missing_vals);

    // Copy the missing value to device
    CUDA_CHECK(cudaMemcpyAsync(d_missing_vals, h_missing_vals, query_length * embedding_vec_size * sizeof(float), cudaMemcpyHostToDevice, stream));
    CUDA_CHECK(cudaStreamSynchronize(stream));

    // Replace the gpu_cache data
    cache -> Replace(d_missing_keys, h_missing_len, d_missing_vals, stream);
    // Wait for stream to complete
    CUDA_CHECK(cudaStreamSynchronize(stream));


    CUDA_CHECK(cudaFree(d_query_keys));
    CUDA_CHECK(cudaFree(d_vals_retrieved));
    CUDA_CHECK(cudaFree(d_missing_keys));
    CUDA_CHECK(cudaFree(d_missing_vals));
    CUDA_CHECK(cudaFree(d_missing_index));
    CUDA_CHECK(cudaFree(d_missing_len));

    delete h_query_keys;
    delete h_vals_retrieved;
    delete h_missing_keys;
    delete h_missing_index;
    delete h_missing_vals;
}
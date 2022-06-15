#pragma once

#include <nv_gpu_cache.hpp>
#include <unordered_set>
#include <unordered_map>
#include <cstdlib>
#include <unistd.h>
#include <cache_base.hpp>
#include <type_def.hpp>
#include <configs.hpp>


class GPUCache:public  CacheBase{
public:
    static const K_type empty_key = std::numeric_limits<K_type>::max();

    GPUCache(const size_t set_size, const size_t set_associativity, const size_t slab_size, const size_t embedding_vec_size);
    using Cache_ = nv_gpu_cache::gpu_cache<K_type, uint64_t, empty_key, SET_ASSOCIATIVITY, SLAB_SIZE>;

    Cache_ * cache;

    void read(const SparseInput& in_keys, D_type* out_data);
    void write();    
};



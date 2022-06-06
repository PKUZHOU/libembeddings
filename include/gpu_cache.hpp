#pragma once
#include <cache_base.hpp>

class GPUCache:public  CacheBase{
public:
    GPUCache(const size_t set_size, const size_t way_size, const size_t block_size);
    void read(const size_t & entry_index);
    void write();    
};
#pragma once

#include<cache_base.hpp>
#include<type_def.hpp>
#include<vector>

using namespace std;

class PSCache:public CacheBase{
private:
    D_type * data_;
    size_t * tag_;
    bool * valid_;

    size_t set_offset;
    size_t way_offset;

public:

    PSCache(const size_t set_size, const size_t set_associativity, const size_t embedding_vec_size);

    void read(const SparseInput& query_keys, D_type* out_data);

    ~PSCache();    
};
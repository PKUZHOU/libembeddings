#pragma once

#include<cache_base.hpp>
#include<type_def.hpp>


class PSCache:public CacheBase{
private:
    D_type * data = nullptr;
    size_t * tag;
    bool * valid;

    size_t set_offset;
    size_t way_offset;

public:

    PSCache(const size_t set_size, const size_t way_size, const size_t block_size);

    void read(const size_t& entry_index);
    ~PSCache();    
};
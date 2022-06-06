#pragma once
#include <stddef.h>
#include<type_def.hpp>

class CacheBase{
private:
    CacheBase * next_level_cache_;
    size_t set_size_;
    size_t way_size_;
    size_t block_size_;
    size_t emb_dim_;
    bool enable_;
public:

    size_t get_block_size(){
        return block_size_;
    }

    size_t get_set_size(){
        return set_size_;
    }

    size_t get_way_size(){
        return way_size_;
    }

    size_t get_emb_dim(){
        return emb_dim_;
    }

    CacheBase(const size_t set_size, const size_t way_size, const size_t block_size){
        set_size_ = set_size;
        way_size_ = way_size;
        block_size_ = block_size;
        emb_dim_ = block_size;  // force equal
        next_level_cache_ = nullptr;
    };

    void set_enable(bool enable){
        enable_ = enable;
    }
    
    void set_next_level_cache(CacheBase * next_level_cache);
    virtual void read(const size_t & entry_index) = 0;
    // virtual void write() = 0;

    ~CacheBase() = default;
};
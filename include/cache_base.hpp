#pragma once
#include <stddef.h>
#include<type_def.hpp>

class CacheBase{
private:
    CacheBase * next_level_cache_;
    size_t set_size_;
    size_t set_associativity_;
    size_t slab_size_;
    size_t emb_dim_;
    bool enable_;
public:

    size_t get_set_associativity(){
        return set_associativity_;
    }

    size_t get_set_size(){
        return set_size_;
    }

    size_t get_slab_size(){
        return slab_size_;
    }

    size_t get_emb_dim(){
        return emb_dim_;
    }

    CacheBase(const size_t set_size, const size_t set_associativity, const size_t slab_size, const size_t embedding_vec_size){
        set_size_ = set_size;
        set_associativity_ = set_associativity;
        slab_size_ = slab_size;
        emb_dim_ = embedding_vec_size;
        next_level_cache_ = nullptr;
    };

    CacheBase(const size_t set_size, const size_t set_associativity, const size_t embedding_vec_size){
        set_size_ = set_size;
        set_associativity_ = set_associativity;
        emb_dim_ = embedding_vec_size;
        next_level_cache_ = nullptr;
    };


    void set_enable(bool enable){
        enable_ = enable;
    }
    
    void set_next_level_cache(CacheBase * next_level_cache);

    CacheBase* get_next_level_cache();

    virtual void Query(const SparseInput& in_queries, D_type* output) = 0;
    
    // virtual void write() = 0;

    ~CacheBase() = default;
};
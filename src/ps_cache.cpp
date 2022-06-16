#include <ps_cache.hpp>
#include <memory.h>
#include <type_def.hpp>
#include <assert.h>

PSCache::PSCache(const size_t set_size, const size_t set_associativity, const size_t embedding_vec_size):CacheBase(set_size, set_associativity, embedding_vec_size){
    size_t cache_entries = get_set_size() *  get_set_associativity();   

    size_t cache_size = cache_entries * get_emb_dim();
    data_ = new D_type[cache_size];
    tag_ = new size_t[cache_entries];
    valid_ = new bool[cache_entries];
    LRU_bit_ = new bool[get_set_size()];

    memset(valid_, 0, sizeof(bool)*cache_entries);
    memset(LRU_bit_, 0, sizeof(bool)*get_set_size());
    set_offset = set_associativity * embedding_vec_size;
}


void PSCache::Replace(const SparseInput& missing_keys, D_type* missing_data){
    const size_t missing_len = missing_keys.indices.size();
    const size_t n_ways = get_set_associativity();
    auto emb_dim = get_emb_dim();
    assert(n_ways == 2);

    for(int idx = 0; idx < missing_len; idx ++){
        size_t key = missing_keys.indices[idx];  
        size_t set_index = key % get_set_size();        
        size_t* target_tag_set = &tag_[set_index * n_ways];
        bool& LRU_bit = LRU_bit_[set_index];
        if(LRU_bit == 0){
            // replace the second way
            memcpy(data_ + set_index * set_offset + emb_dim,  missing_data + idx * emb_dim, emb_dim*sizeof(D_type));
            tag_[set_index * n_ways + 1] = key;
            LRU_bit = 1;
        }else{
            // replace the first way
            memcpy(data_ + set_index * set_offset,  missing_data + idx * emb_dim, emb_dim*sizeof(D_type));
            tag_[set_index * n_ways] = key;
            LRU_bit = 0;
        }        
    }
}


void PSCache::Query(const SparseInput& query_keys, D_type * out_data){
    const int query_len = query_keys.indices.size();
    auto n_ways = get_set_associativity();
    auto emb_dim = get_emb_dim();

    K_type* missing_keys = new K_type[query_len];
    D_type* vals_retrieved = new D_type[query_len * get_emb_dim()];
    D_type* vals_missing = new D_type[query_len * get_emb_dim()];
    size_t hit_len = 0;
    size_t miss_len = 0;

    for(int query_idx = 0; query_idx < query_len; query_idx ++){
        size_t key = query_keys.indices[query_idx];  
        size_t set_index = key % get_set_size();
        
        size_t* target_tag_set = &tag_[set_index * n_ways];
        bool find_entry = false;

        //traverse all the ways
        for(int way_idx = 0; way_idx < n_ways; way_idx++){
            bool is_valid = valid_[set_index * n_ways + way_idx];
            if(is_valid){
                size_t & tag = tag_[set_index * n_ways + way_idx];
                if(tag == key){
                    find_entry = true;
                    memcpy(vals_retrieved + hit_len * emb_dim, data_ + set_index * set_offset + way_idx * emb_dim, emb_dim*sizeof(D_type));
                    hit_len++;
                    //update the LRU bit, only for two-way cache
                    LRU_bit_[set_index] = way_idx;
                }
            }
        }
        if(!find_entry){
            // cache miss
            missing_keys[miss_len++] = key;
        }
    }
    
    SparseInput sp_missing_keys;
    for(size_t missing_idx = 0; missing_idx < miss_len; missing_idx++){
        sp_missing_keys.indices.push_back(missing_keys[missing_idx]);
    }

    //query the next level cache
    CacheBase* next_level_cache = get_next_level_cache();
    next_level_cache->Query(sp_missing_keys,vals_missing);
    
    //replace data
    Replace(sp_missing_keys,vals_missing);

    //copy out the results
    memcpy(out_data, vals_retrieved, sizeof(D_type) * emb_dim * hit_len);
    memcpy(out_data + hit_len * emb_dim, vals_missing, sizeof(D_type) * emb_dim * miss_len);

    delete missing_keys;
    delete vals_retrieved;
    delete vals_missing;
}


PSCache::~PSCache(){
    if(data_)
    {
        delete data_;
        data_ = nullptr;
    }
    if(tag_){
        delete tag_;
        tag_ = nullptr;
    }
    if(valid_){
        delete valid_;
        valid_ = nullptr;
    }
}
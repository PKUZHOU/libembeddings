#include <ps_cache.hpp>
#include <memory.h>
#include<type_def.hpp>

PSCache::PSCache(const size_t set_size, const size_t way_size, const size_t block_size):CacheBase(set_size, way_size, block_size){
    size_t cache_entries = get_set_size() *  get_way_size();
    size_t cache_size = cache_entries * get_block_size();
    data = new D_type[cache_size];
    tag = new size_t[cache_entries];
    valid = new bool[cache_entries];
    memset(valid, 0, sizeof(bool)*cache_size);
}


void PSCache::read(const size_t& index){
    // get way
    size_t set_index = index / get_way_size();
    size_t way_offset = index % get_way_size(); 

    bool &is_valid = valid[set_index * set_offset + way_offset];
    if(is_valid){
        // get tag
        size_t& entry_tag = tag[set_index * set_offset + way_offset];
        if(entry_tag == index){

        }else{
            // next_level_cache_->read(index);
            // replacement
        }
    }else{
        // next_level_cache_->read(index);
        //replacement
        is_valid = true;
    }

}


PSCache::~PSCache(){
    if(data)
    {
        delete data;
        data = nullptr;
    }
    if(tag){
        delete tag;
        tag = nullptr;
    }
    if(valid){
        delete valid;
        valid = nullptr;
    }
}
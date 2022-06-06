#include<cache_base.hpp>
#include<iostream>

void CacheBase::set_next_level_cache(CacheBase * next_level_cache){
    if(next_level_cache){
        next_level_cache_ = next_level_cache;
    }
    else{
        std::cerr<<"Null Next Level Cache!"<<std::endl;        
    }
}
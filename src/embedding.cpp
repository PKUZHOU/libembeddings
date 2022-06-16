#include <embedding.hpp>
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <string>

// template<typename T>
Embedding::Embedding(const size_t total_entries, const size_t emb_dim, const std::string cache_cfg_yaml){
    total_entries_ = total_entries;
    cache_cfg_yaml_ = cache_cfg_yaml;

    YAML::Node config;
    try{
        config = YAML::LoadFile(cache_cfg_yaml);
    }catch(YAML::BadFile &e){
        std::cerr<<"Config File Not Open"<<std::endl;
    }
    bool enable_gpu_cache;
    size_t gpu_set_size;
    size_t gpu_set_associativity;
    size_t gpu_slab_size;

    bool enable_ps_cache;
    size_t ps_set_size;
    size_t ps_set_associativity;
    size_t ps_slab_size;

    bool enable_file_cache;
    size_t file_set_size;
    size_t file_set_associativity;
    size_t file_slab_size; 
    std::string cache_dir;

    try{
        enable_gpu_cache = config["gpu_cache"]["enable"].as<bool>();
        gpu_set_size = config["gpu_cache"]["set_size"].as<size_t>();
        gpu_set_associativity = config["gpu_cache"]["set_associativity"].as<size_t>();
        gpu_slab_size = config["gpu_cache"]["slab_size"].as<size_t>();

        enable_ps_cache = config["ps_cache"]["enable"].as<bool>();
        ps_set_size = config["ps_cache"]["set_size"].as<size_t>();
        ps_set_associativity = config["ps_cache"]["set_associativity"].as<size_t>();

        enable_file_cache = config["file_cache"]["enable"].as<bool>();
        file_set_size = config["file_cache"]["set_size"].as<size_t>();
        file_set_associativity = config["file_cache"]["set_associativity"].as<size_t>();

        cache_dir = config["file_cache"]["cache_dir"].as<string>();

    }catch(YAML::TypedBadConversion<std::string> &e){
        std::cerr<<"Error in parsing configs"<<std::endl;
    }

    //generate cache hierachy
    if(enable_gpu_cache){
        gpu_cache = new GPUCache(gpu_set_size,gpu_set_associativity,gpu_slab_size,emb_dim);
    }
    if(enable_ps_cache){
        ps_cache = new PSCache(ps_set_size, ps_set_associativity, emb_dim);
    }
    if(enable_file_cache){
        file_cache = new FileCache(file_set_size, 1, emb_dim, cache_dir); //set size = total entries
    }


    if(enable_gpu_cache){
        if(enable_ps_cache){
            gpu_cache->set_next_level_cache(ps_cache);
        }
        else if(enable_file_cache){
            gpu_cache->set_next_level_cache(file_cache);
        }
    }
    else if(enable_ps_cache){
        if(enable_file_cache){
            ps_cache->set_next_level_cache(file_cache);
        }
    }
    if(enable_ps_cache && enable_file_cache){
        ps_cache->set_next_level_cache(file_cache);
    } 
}


void Embedding::Query(const SparseInput & in_keys, D_type * out_data){
    // Query from the cache hierachy
    if(gpu_cache){
        gpu_cache->Query(in_keys, out_data);
    }
    else if(ps_cache){
        ps_cache->Query(in_keys, out_data);
    }
    else if(file_cache){
        file_cache->Query(in_keys, out_data);
    }else{
        std::cerr<<"No Cache!"<<std::endl;
    }
}

Embedding::~Embedding(){
    if(gpu_cache){
        delete(gpu_cache);
    }
    if(ps_cache){
        delete(ps_cache);
    }
    if(file_cache){
        delete(file_cache);
    }
}

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
    size_t gpu_way_size;
    size_t gpu_block_size;

    bool enable_ps_cache;
    size_t ps_set_size;
    size_t ps_way_size;
    size_t ps_block_size;

    bool enable_file_cache;
    size_t file_set_size;
    size_t file_way_size;
    size_t file_block_size; 
    std::string cache_dir;

    try{
        enable_gpu_cache = config["gpu_cache"]["enable"].as<bool>();
        gpu_set_size = config["gpu_cache"]["set_size"].as<size_t>();
        gpu_way_size = config["gpu_cache"]["way_size"].as<size_t>();
        gpu_block_size = config["gpu_cache"]["block_size"].as<size_t>();

        enable_ps_cache = config["ps_cache"]["enable"].as<bool>();
        ps_set_size = config["ps_cache"]["set_size"].as<size_t>();
        ps_way_size = config["ps_cache"]["way_size"].as<size_t>();
        ps_block_size = config["ps_cache"]["block_size"].as<size_t>();

        enable_file_cache = config["file_cache"]["enable"].as<bool>();
        file_set_size = config["file_cache"]["set_size"].as<size_t>();
        file_way_size = config["file_cache"]["way_size"].as<size_t>();
        file_block_size = config["file_cache"]["block_size"].as<size_t>();
        cache_dir = config["file_cache"]["cache_dir"].as<string>();


    }catch(YAML::TypedBadConversion<std::string> &e){
        std::cerr<<"Error in parsing configs"<<std::endl;
    }

    //generate cache hierachy
    gpu_cache = new GPUCache(gpu_set_size,gpu_way_size,gpu_block_size);
    gpu_cache->set_enable(enable_gpu_cache);

    ps_cache = new PSCache(ps_set_size, ps_way_size, ps_block_size);
    ps_cache->set_enable(enable_ps_cache);

    file_cache = new FileCache(file_set_size, file_way_size, file_block_size, cache_dir);
    file_cache->set_enable(enable_file_cache);    

    gpu_cache->set_next_level_cache(ps_cache);
    ps_cache->set_next_level_cache(file_cache);
}

void Embedding::read(const size_t& query_index, D_type * out_data){
    // read from the cache hierachy
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

#pragma once
#include <stddef.h>
#include <string>
#include <vector>
#include <type_def.hpp>
#include <gpu_cache.hpp>
#include <ps_cache.hpp>
#include <file_cache.hpp>

using namespace std;


class Embedding{
private:
    size_t total_entries_;
    std::string cache_cfg_yaml_;
    GPUCache* gpu_cache = nullptr;
    PSCache* ps_cache = nullptr;
    FileCache* file_cache = nullptr;

public:
    Embedding(const size_t total_entries, const size_t emb_dim, const std::string cache_cfg_yaml);
    void read(const SparseInput& in_keys, D_type * out_data);
    
    ~Embedding();

 };


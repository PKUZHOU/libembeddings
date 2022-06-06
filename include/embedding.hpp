#pragma once
#include <stddef.h>
#include <string>
#include <vector>
#include <type_def.hpp>
#include <gpu_cache.hpp>
#include <ps_cache.hpp>
#include <file_cache.hpp>

using namespace std;

struct SparseInput
{   
    vector<size_t> indices;   
};

class Embedding{
private:
    size_t total_entries_;
    std::string cache_cfg_yaml_;
    GPUCache* gpu_cache;
    PSCache* ps_cache;
    FileCache* file_cache;

public:
    Embedding(const size_t total_entries, const size_t emb_dim, const std::string cache_cfg_yaml);
    
    void read(const size_t & query_index, D_type * out_data);
    
    ~Embedding();

 };


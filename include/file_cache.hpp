#pragma once
#include<cache_base.hpp>
#include<string>
#include<type_def.hpp>

using namespace std;

class FileCache:public CacheBase{
public:
    string file_cache_dir_;
    FILE * value_file = nullptr;
    FILE * key_file = nullptr;

    size_t vec_offset_bytes;

    char* read_buffer;

    FileCache(const size_t set_size, const size_t set_associativity, const size_t embedding_vec_size, const string & file_cache_dir);

    void Query(const SparseInput& query_keys, D_type* out_data);
    ~FileCache();
};
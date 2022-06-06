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

    size_t way_offset_bytes;

    char* read_buffer;

    FileCache(const size_t set_size, const size_t way_size, const size_t block_size, const string & file_cache_dir);

    void read(const size_t& entry_index);
    ~FileCache();
};
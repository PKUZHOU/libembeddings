#include <file_cache.hpp>
#include <iostream>
#include<stdio.h>
#include<type_def.hpp>

inline bool file_exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

FileCache::FileCache(const size_t set_size, const size_t set_associativity, 
        const size_t slab_size, const size_t embedding_vec_size,const string & file_cache_dir):CacheBase(set_size,set_associativity,slab_size,embedding_vec_size){
    /*
    file_cache_dir_ = file_cache_dir;
    std::string value_file_path = file_cache_dir_ + "/value.bin";
    if(!file_exists(value_file_path)){
        std::cerr<<value_file<<" Does Not Exist"<<std::endl;
    }

    std::string key_file_path = file_cache_dir_ + "/key.bin";
    if(!file_exists(key_file_path)){
        std::cerr<<key_file<<" Does Not Exist" << std::endl;
    }

    this->value_file = fopen(value_file_path.c_str(),"r");
    this->key_file = fopen(key_file_path.c_str(),"r");

    // check file size
    fseek(this->value_file,0,SEEK_END);
    int n_value_elements = ftell(this->value_file);
    fseek(this->value_file,0,SEEK_SET);

    fseek(this->key_file,0,SEEK_END);
    int n_key_elements = ftell(this->key_file);
    fseek(this->key_file,0,SEEK_SET);

    // compute the way offset
    way_offset_bytes =  get_block_size() * get_way_size() * sizeof(D_type);

    // allocate read buffer
    read_buffer = new char[get_emb_dim() * sizeof(D_type)];
    */
}

void FileCache::read(const size_t& entry_index){
    /*
    int target_set_size = entry_index / (get_block_size() * get_way_size());
    if(target_set_size > get_set_size()){
        std::cerr<<"File Cache Error"<<std::endl;
    }
    size_t entry_start_byte = target_set_size * way_offset_bytes;
    
    fseek(this->value_file,0,entry_start_byte);
    fread(read_buffer, sizeof(D_type), get_emb_dim(), this->value_file);
    */
}


FileCache::~FileCache(){
    fclose(value_file);
    fclose(key_file);
    delete read_buffer;
}
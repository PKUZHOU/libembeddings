#pragma once
#include<embedding.hpp>
#include<string>
#include<iostream>
#include<type_def.hpp>

class EmbeddingBag{
private:
    size_t num_embeddings_;
    size_t embedding_dim_;
    std::string mode_;
    Embedding* embedding_table;

public:
    EmbeddingBag(const size_t num_embeddings, const size_t embedding_dim, const std::string mode, const std::string cache_cfg_yaml);
    
    void load_weights();

    void forward(const SparseInput & input, D_type * output);
    
    
    ~EmbeddingBag();

};
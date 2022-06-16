#include <embedding_bag.hpp>

EmbeddingBag::EmbeddingBag(const size_t num_embeddings, const size_t embedding_dim, const std::string mode, const std::string cache_cfg_yaml) : num_embeddings_(num_embeddings), embedding_dim_(embedding_dim), mode_(mode) {
    embedding_table = new Embedding(num_embeddings, embedding_dim, cache_cfg_yaml);
}

void EmbeddingBag::forward(const SparseInput & input, D_type * output){
    const size_t n_queries = input.indices.size();
    // \sum_{i=0}^{n_queries-1} embedding(indices[i]);
    if(this->mode_ == "sum"){
        // should call a GPU kernel
        D_type* tmp_entry_data = new D_type[n_queries * embedding_dim_];
        embedding_table->Query(input, tmp_entry_data);
        //sum
        
    }
    else{
        std::cerr<<"Mode Not Supported!"<<std::endl;
    }
}

EmbeddingBag::~EmbeddingBag(){
    if(embedding_table){
        delete embedding_table;
    }
}


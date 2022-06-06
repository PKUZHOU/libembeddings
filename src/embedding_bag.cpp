#include <embedding_bag.hpp>

EmbeddingBag::EmbeddingBag(const size_t num_embeddings, const size_t embedding_dim, const std::string mode, const std::string cache_cfg_yaml) 
                                : num_embeddings_(num_embeddings), embedding_dim_(embedding_dim), mode_(mode) {

    embedding_table = new Embedding(num_embeddings, embedding_dim, cache_cfg_yaml);
}

void EmbeddingBag::forward(const SparseInput & input){
    const size_t n_queries = input.indices.size();
    // \sum_{i=0}^{n_queries-1} embedding(indices[i]);
    if(this->mode_ == "sum"){
        // should call a GPU kernel
        D_type* tmp_entry_data = new D_type[embedding_dim_];

        for(size_t query_idx = 0; query_idx < n_queries; query_idx ++){
            embedding_table->read(input.indices[query_idx], tmp_entry_data);
        }
    }
    else{
        std::cerr<<"Mode Not Supported!"<<std::endl;
    }
}

void EmbeddingBag::init_weights(){
    // fill a generated table?
}

EmbeddingBag::~EmbeddingBag(){
    if(embedding_table){
        delete embedding_table;
    }
}


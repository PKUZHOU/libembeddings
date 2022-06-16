#include <embedding_bag.hpp>
#include <memory>
#include <string>
#include <stdlib.h>
#include <util.hpp>

using namespace std;


void fill_input(SparseInput& input, const size_t
                                     max_idx, int n_queries){
    for(int i = 0;i<n_queries;i++){
        input.indices.push_back(random(max_idx));
    }
}

void run_test(){
    string cache_cfg_yaml = "./config/cache_cfg.yaml";
    string mode = "sum"; 

    CUDA_CHECK(cudaSetDevice(0));

    // Hardcode some parameters now
    const size_t emb_size = 10000000;
    const size_t embedding_vec_size = 64;
    const size_t query_length = 16;
    // const size_t num_threads = 1;

    EmbeddingBag embedding_bag(emb_size,embedding_vec_size,mode,cache_cfg_yaml);

    SparseInput input;
    fill_input(input, emb_size, query_length);

    D_type* output;
    output = new D_type[embedding_vec_size];

    // TODO: load the inputs to GPU 
    embedding_bag.forward(input, output);
    
    for(size_t i = 0;i<query_length;i++){
        cout<<output[i]<<",";
    }
    cout<<endl;

}

int main(){
    run_test();
}
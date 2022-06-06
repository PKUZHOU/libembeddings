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
    EmbeddingBag embedding_bag(1024,64,mode,cache_cfg_yaml);

    embedding_bag.init_weights();

    SparseInput input;
    fill_input(input, 1024, 64);

    embedding_bag.forward(input);
}

int main(){
    run_test();
}
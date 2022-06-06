
# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCLIST)

add_library(embedding_bag_shared SHARED   
    embedding_bag.cpp 
    embedding.cpp 
    cache_base.cpp 
    gpu_cache.cpp 
    ps_cache.cpp 
    file_cache.cpp)

target_link_libraries(embedding_bag_shared yaml-cpp)

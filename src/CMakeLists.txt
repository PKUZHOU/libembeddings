
# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCLIST)
add_definitions("-Wall -g")

add_library(embedding_bag_shared SHARED   
    embedding_bag.cpp 
    embedding.cpp 
    cache_base.cpp 
    gpu_cache.cpp 
    ps_cache.cpp 
    file_cache.cpp
    )

target_compile_features(embedding_bag_shared PUBLIC cxx_std_11)
target_link_libraries(embedding_bag_shared  yaml-cpp gpu_cache)
# target_link_libraries(embedding_bag_shared PUBLIC OpenMP::OpenMP_CXX)
# set_target_properties(embedding_bag_shared PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
# set_target_properties(embedding_bag_shared PROPERTIES CUDA_ARCHITECTURES OFF)

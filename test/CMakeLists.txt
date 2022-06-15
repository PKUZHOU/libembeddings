cmake_minimum_required(VERSION 3.10)

add_definitions("-Wall -g")


add_executable(embedding_bag_test embedding_test.cpp)


target_link_libraries(embedding_bag_test embedding_bag_shared gpu_cache)



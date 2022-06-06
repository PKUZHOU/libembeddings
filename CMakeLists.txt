CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(embedding_bag)


add_subdirectory(3rd)

include_directories(3rd/yaml-cpp/include)
include_directories(include)

add_subdirectory(src)
add_subdirectory(test)





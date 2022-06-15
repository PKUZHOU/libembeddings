CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(embedding_bag LANGUAGES CXX CUDA)


option(WITH_GPU  "Build with GPU"  ON)

# find_package(CUDA REQUIRED)
# find_package(CUDNN REQUIRED)

if (WITH_GPU)
    set(CUDA_PATH "/usr/local/cuda-11.6" CACHE PATH "The cuda library root")
    include_directories(${CUDA_PATH}/include)
    link_directories(${CUDA_PATH}/lib64)
endif()


#YAML
add_subdirectory(3rd)
include_directories(3rd/yaml-cpp/include)
include_directories(gpu_cache/include)


include_directories(include)

add_subdirectory(gpu_cache/src)
# add_subdirectory(gpu_cache/test)

add_subdirectory(src)
add_subdirectory(test)





# 
# Copyright (c) 2021, NVIDIA CORPORATION.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.8)

add_definitions("-g")

add_library(gpu_cache SHARED nv_gpu_cache.cu)
target_compile_features(gpu_cache PUBLIC cxx_std_11)
set_target_properties(gpu_cache PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
set_target_properties(gpu_cache PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
set_target_properties(gpu_cache PROPERTIES CUDA_ARCHITECTURES OFF)

target_link_libraries(gpu_cache cudart cuda)

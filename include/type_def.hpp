#pragma once
#include <cstdlib>
#include <unistd.h>
#include <limits>
#include <vector>

typedef float D_type;
typedef long long K_type;
using ref_counter_type = unsigned long long;

struct SparseInput
{   
    std::vector<size_t> indices;   
};

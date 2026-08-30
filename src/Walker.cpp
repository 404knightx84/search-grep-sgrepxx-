#include "Walker.hpp"
#include <iostream>
#include <fnmatch.h>

namespace fs = std::filesystem;

Walker::Walker(const Config& cfg) : cfg_(cfg) {}

bool Walker::passes_filters(const fs::path& p, bool is_dir) const {
    if (cfg_.type_filter == 'f' && is_dir) return false;
    if (cfg_.type_filter == 'd' && !is_dir) return false;

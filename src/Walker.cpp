#include "Walker.hpp"
#include <iostream>
#include <fnmatch.h>

namespace fs = std::filesystem;

Walker::Walker(const Config& cfg) : cfg_(cfg) {}

bool Walker::passes_filters(const fs::path& p, bool is_dir) const {
    if (cfg_.type_filter == 'f' && is_dir) return false;
    if (cfg_.type_filter == 'd' && !is_dir) return false;
    if (cfg_.name_filter) {
        // std::filesystem gives us the filename directly -- no manual
        // strrchr('/') hunting like the C version needed.
        const std::string filename = p.filename().string();

        // fnmatch() is still the simplest way to do glob matching
        // ("*.cpp") in C++; there's no std::filesystem equivalent.
        if (fnmatch(cfg_.name_filter->c_str(), filename.c_str(), 0) != 0) {
            return false;
        }
    }
    return true;
}

void Walker::walk(const FileVisitor& visitor) const {
    fs::path root(cfg_.path);

    std::error_code ec;
    if (!fs::exists(root, ec)) {
        std::cerr << "sgrepxx: path does not exist: " << cfg_.path << "\n";
        return;
    }

    // Single file (not a directory): just check it directly.
    if (fs::is_regular_file(root, ec)) {
        if (passes_filters(root, false)) visitor(root);
        return;
    }

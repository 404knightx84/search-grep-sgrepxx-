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
    if (!fs::is_directory(root, ec)) return;

    // recursive_directory_iterator does exactly what walk_recursive()
    // did by hand in the C version (opendir/readdir/recurse), but as
    // one line, with RAII cleanup guaranteed even if an exception is
    // thrown partway through.
    //
    // skip_permission_denied means one unreadable subdirectory doesn't
    // crash the whole walk -- it just gets skipped, matching how real
    // `find`/`grep -r` behave.
    auto opts = fs::directory_options::skip_permission_denied;

    if (cfg_.recursive) {
        for (const auto& entry : fs::recursive_directory_iterator(root, opts, ec)) {
            bool is_dir = entry.is_directory(ec);
            if (passes_filters(entry.path(), is_dir)) {
                if (is_dir && cfg_.type_filter != 'd') continue; // skip dirs unless -type d
                if (!is_dir || cfg_.type_filter == 'd') visitor(entry.path());
            }
        }
    } 

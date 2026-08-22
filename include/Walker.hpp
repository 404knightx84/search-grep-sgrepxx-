#pragma once

#include "Config.hpp"
#include <filesystem>
#include <functional>


using FileVisitor = std::function<void(const std::filesystem::path&)>;

class Walker {
public:
    explicit Walker(const Config& cfg);

    // Walks cfg.path (recursively, if it's a directory and cfg.recursive
    // is set), calling visitor for every entry that passes the filters.
    void walk(const FileVisitor& visitor) const;

private:
    const Config& cfg_;

    bool passes_filters(const std::filesystem::path& p, bool is_dir) const;
};

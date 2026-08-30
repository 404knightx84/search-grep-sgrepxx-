#include "Config.hpp"
#include "Matcher.hpp"
#include "Walker.hpp"

#include <iostream>
#include <fstream>
#include <string>

// Searches one file's contents line by line, printing matches.
// Note: no fopen/fclose pair to manage -- std::ifstream closes itself
// automatically when it goes out of scope (RAII), even if we return
// early or an exception is thrown mid-function.
static void search_file(const std::filesystem::path& path, const Config& cfg, const Matcher& matcher) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "sgrepxx: cannot open '" << path.string() << "'\n";
        return;
    }

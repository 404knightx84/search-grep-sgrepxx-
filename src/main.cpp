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
    std::string line;
    int line_no = 0;

    while (std::getline(file, line)) {
        line_no++;

        if (matcher.contains(line)) {
            if (cfg.filenames_only) {
                std::cout << path.string() << "\n";
                return; // no need to keep scanning this file
            }
            if (cfg.show_line_numbers) {
                std::cout << path.string() << ":" << line_no << ":" << line << "\n";
            } else {
                std::cout << path.string() << ":" << line << "\n";
            }
        }
    }
}

#pragma once

#include <string>
#include <optional>


struct Config {
    std::string pattern;
    std::string path = ".";
    std::optional<std::string> name_filter;  // -name "*.cpp"
    std::optional<char> type_filter;         // -type f|d

    bool case_insensitive = false;
    bool show_line_numbers = false;
    bool filenames_only = false;
    bool recursive = true;
};

// Parses argv into a Config. Throws std::runtime_error on bad input,
// caught in main() to print usage and exit cleanly.
Config parse_args(int argc, char* argv[]);

void print_usage(const std::string& prog_name);

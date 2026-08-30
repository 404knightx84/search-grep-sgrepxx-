#include "Config.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

void print_usage(const std::string& prog_name) {
    std::cerr <<
        "Usage: " << prog_name << " [options] PATTERN [PATH]\n\n"
        "Search for PATTERN in files under PATH (default: current directory).\n\n"
        "Options:\n"
        "  -i            case-insensitive matching\n"
        "  -n            show line numbers\n"
        "  -l            only print filenames that contain a match\n"
        "  -r            recurse into subdirectories (default on for dirs)\n"
        "  -name GLOB    only search files whose name matches GLOB (e.g. \"*.cpp\")\n"
        "  -type f|d     only consider files (f) or directories (d)\n"
        "  -h            show this help message\n\n"
        "Examples:\n"
        "  " << prog_name << " TODO ./src\n"
        "  " << prog_name << " -i -n \"error\" .\n";
}

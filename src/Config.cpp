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
Config parse_args(int argc, char* argv[]) {
    // Copy argv into a vector<string> up front. This is the idiomatic
    // C++ move: no more manual strcmp() everywhere, and strings own
    // their own memory instead of pointing into argv's raw char*s.
    std::vector<std::string> args(argv + 1, argv + argc);

    Config cfg;
    std::vector<std::string> positionals;

    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& a = args[i];

        if (a == "-i") {
            cfg.case_insensitive = true;
        } else if (a == "-n") {
            cfg.show_line_numbers = true;
        } else if (a == "-l") {
            cfg.filenames_only = true;
        } else if (a == "-r") {
            cfg.recursive = true;
        } else if (a == "-h") {
            print_usage(argv[0]);
            std::exit(0);
        } else if (a == "-name") {
            if (i + 1 >= args.size()) throw std::runtime_error("-name requires an argument");
            cfg.name_filter = args[++i];
        } else if (a == "-type") {
            if (i + 1 >= args.size() || (args[i + 1] != "f" && args[i + 1] != "d"))
                throw std::runtime_error("-type requires 'f' or 'd'");
            cfg.type_filter = args[++i][0];
        } else if (!a.empty() && a[0] == '-') {
            throw std::runtime_error("unknown option '" + a + "'");
        } else {
            if (positionals.size() >= 2) throw std::runtime_error("too many arguments");
            positionals.push_back(a);
        }
    }

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

int main(int argc, char* argv[]) {
    Config cfg;
    try {
        cfg = parse_args(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "sgrepxx: " << e.what() << "\n";
        print_usage(argc > 0 ? argv[0] : "sgrepxx");
        return 1;
    }

    Matcher matcher(cfg.pattern, cfg.case_insensitive);
    Walker walker(cfg);

    // A lambda closure lets us pass `cfg` and `matcher` into the visitor
    // without global variables -- something the C version couldn't do
    // cleanly with a plain function pointer callback.
    walker.walk([&](const std::filesystem::path& path) {
        search_file(path, cfg, matcher);
    });

    return 0;
}

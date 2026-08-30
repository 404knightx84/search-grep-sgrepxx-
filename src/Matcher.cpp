#include "Matcher.hpp"
#include <cctype>

Matcher::Matcher(std::string pattern, bool case_insensitive)
    : pattern_(std::move(pattern)), case_insensitive_(case_insensitive) {
    lps_ = build_lps();
}

char Matcher::normalize(char c, bool ci) {
    return ci ? static_cast<char>(std::tolower(static_cast<unsigned char>(c))) : c;
}

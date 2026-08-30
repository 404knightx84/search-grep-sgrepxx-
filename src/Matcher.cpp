#include "Matcher.hpp"
#include <cctype>

Matcher::Matcher(std::string pattern, bool case_insensitive)
    : pattern_(std::move(pattern)), case_insensitive_(case_insensitive) {
    lps_ = build_lps();
}

char Matcher::normalize(char c, bool ci) {
    return ci ? static_cast<char>(std::tolower(static_cast<unsigned char>(c))) : c;
}

std::vector<int> Matcher::build_lps() const {
    const int m = static_cast<int>(pattern_.size());
    std::vector<int> lps(m, 0);

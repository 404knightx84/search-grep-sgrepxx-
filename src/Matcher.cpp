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

    int len = 0;
    int i = 1;
    while (i < m) {
        if (normalize(pattern_[i], case_insensitive_) == normalize(pattern_[len], case_insensitive_)) {
            len++;
            lps[i] = len;
            i++;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i] = 0;
            i++;
        }
    }
    return lps;
}

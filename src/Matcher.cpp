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

bool Matcher::contains(const std::string& text) const {
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern_.size());

    if (m == 0) return true;
    if (m > n) return false;

    int i = 0, j = 0;
    while (i < n) {
        if (normalize(text[i], case_insensitive_) == normalize(pattern_[j], case_insensitive_)) {
            i++;
            j++;
            if (j == m) return true;
        } else if (j != 0) {
            j = lps_[j - 1];
        } else {
            i++;
        }
    }
    return false;
}


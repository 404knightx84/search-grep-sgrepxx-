#pragma once

#include <string>
#include <vector>


class Matcher {
public:
    explicit Matcher(std::string pattern, bool case_insensitive);

    // Returns true if the pattern occurs anywhere in `text`.
    bool contains(const std::string& text) const;

private:
    std::string pattern_;
    bool case_insensitive_;
    std::vector<int> lps_;  // longest-prefix-suffix table, built once

    static char normalize(char c, bool ci);
    std::vector<int> build_lps() const;
};

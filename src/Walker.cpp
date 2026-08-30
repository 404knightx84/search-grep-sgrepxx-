#include "Walker.hpp"
#include <iostream>
#include <fnmatch.h>

namespace fs = std::filesystem;

Walker::Walker(const Config& cfg) : cfg_(cfg) {}

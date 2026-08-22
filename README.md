# sgrepxx — the C++ version of sgrep

Same tool as `sgrep` (a `find` + `grep` clone), rebuilt in C++ to show how
the same logic looks with classes, RAII, and the standard library instead
of raw C structs and POSIX calls.

## Build & run

**Option A — plain Makefile (verified working in this environment):**
```bash
make
./sgrepxx -n "TODO" ./src
```

**Option B — CMake (standard syntax, not verified in this sandbox —
test it on your own machine):**
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/sgrepxx -n "TODO" ./src
```

## Usage

Same flags as the C version:
```
Usage: ./sgrepxx [options] PATTERN [PATH]

  -i            case-insensitive matching
  -n            show line numbers
  -l            only print filenames that contain a match
  -r            recurse into subdirectories (default on for dirs)
  -name GLOB    only search files whose name matches GLOB (e.g. "*.cpp")
  -type f|d     only consider files (f) or directories (d)
  -h            show this help message
```

## Project structure

```
sgrepxx/
├── CMakeLists.txt   # standard CMake build (untested here — verify locally)
├── Makefile          # plain g++ build, verified working
├── README.md
├── include/
│   ├── Config.hpp      # struct + parser declaration
│   ├── Walker.hpp        # directory traversal class
│   └── Matcher.hpp         # KMP matcher class
└── src/
    ├── Config.cpp        # argv -> Config, via exceptions not exit()
    ├── Walker.cpp          # uses std::filesystem, not opendir/readdir
    ├── Matcher.cpp           # KMP algorithm as class methods
    └── main.cpp                # ties it together with a lambda callback
```

## What changed vs. the C version — and why it matters

This is the part worth actually studying, not just skimming.

| Concern | C version (`sgrep`) | C++ version (`sgrepxx`) | Why it matters |
|---|---|---|---|
| Directory walking | Manual `opendir`/`readdir`/`lstat`, hand-rolled recursion | `std::filesystem::recursive_directory_iterator` | One line replaces ~30 lines of manual recursion and error handling. `std::filesystem` also handles path-separator differences across platforms for you. |
| String ownership | Raw `char*`, must track who owns/frees each buffer | `std::string`, owns its own memory automatically | Eliminates an entire category of bugs: use-after-free, double-free, forgetting to `free()`. |
| File handles | Manual `fopen()`/`fclose()` pairs; easy to leak on early `return` | `std::ifstream`, closes itself via RAII (destructor runs automatically) | You cannot forget to close a file — the compiler-generated cleanup runs even on exceptions or early returns. |
| Error handling | `exit(1)` scattered through `args.c`, hard to unit-test | Exceptions (`throw std::runtime_error`), caught once in `main()` | Centralizes error handling in one place instead of littering exit points everywhere; also makes `parse_args()` independently testable. |
| Callbacks | Raw function pointer (`file_visitor`), no captured state — had to pass `Config*` manually every call | `std::function` + lambda closure (`[&](path) {...}`) | The lambda in `main.cpp` captures `cfg` and `matcher` automatically. No manual context-passing needed. |
| Matching state | KMP table rebuilt fresh on every call to `kmp_contains()` | `Matcher` class builds its LPS table once in the constructor, reuses it | Real performance difference if you're scanning thousands of lines — you're not redoing the same O(m) setup work every single line. |
| Optional fields | `char type_filter` used `0` as a sentinel for "not set" | `std::optional<char>` | Makes "unset" a real, explicit state instead of an implicit convention you have to remember. |

## Verified correctness

Both `sgrep` (C) and `sgrepxx` (C++) were diffed against real `grep -rn`
output on their own source trees — byte-for-byte identical results, same
methodology used for the C version.

## Suggested next steps (same spirit as the C roadmap, C++ flavored)

1. **Swap KMP for `std::regex`** in `Matcher` — compare API ergonomics
   and runtime performance against your hand-rolled KMP.
2. **Multithreading with `std::async`/`std::thread`** — search multiple
   files in parallel; use a `std::mutex` to guard `std::cout`.
2b. Alternative: use a `std::future` per file and collect results instead
    of printing directly from worker threads — avoids the mutex entirely.
3. **Binary file detection** — check for null bytes before treating a
   file as text, same gap as the C version.
4. **Compare the two codebases directly** in a write-up: line count,
   what's harder/easier to get right in each, where C++ abstractions
   cost performance vs. where they're free (zero-cost abstractions).
   This kind of side-by-side comparison is genuinely good portfolio
   material — it shows you understand tradeoffs, not just syntax.

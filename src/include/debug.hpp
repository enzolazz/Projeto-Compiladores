#pragma once
#ifndef NDEBUG

/* Debug utilities that work only when the project is compiled in debug mode.
 * Variables are printed using << operator and std::cerr.
 */

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define db_print(...)                                                          \
    for (const auto &a : {__VA_ARGS__})                                        \
        std::cout << a;                                                        \
    (std::cout << std::endl)

// Print a message and variables to stderr
#define db_msg(msg, ...)                                                       \
    __debug::varsNamesPrefix("(Debug)",                                        \
                             msg __VA_OPT__(, #__VA_ARGS__, ) __VA_ARGS__)

// Print a message and variables to stderr with line number
#define db_line(msg, ...)                                                      \
    __debug::varsNamesPrefix("(Debug) LINE " STRINGIZE(__LINE__), msg __VA_OPT__(, #__VA_ARGS__, ) __VA_ARGS__)

// Print a message and variables to stderr with this file's name and line number
#define db_file(msg, ...)                                                      \
    __debug::varsNamesPrefix("(Debug) " __FILE__ ":" STRINGIZE(__LINE__), msg __VA_OPT__(, #__VA_ARGS__, ) __VA_ARGS__)

// Don't use this namespace directly in your code
namespace __debug {
void varsNames(std::size_t i, const std::vector<std::string> &names);

template <typename T, typename... Var>
void varsNames(std::size_t i, const std::vector<std::string> &names, T v,
               Var... vars) {
    std::cerr << std::boolalpha << names[i] << '=' << v << ",, ";
    varsNames(i + 1, names, vars...);
}

template <typename... Var>
void varsNamesPrefix(const char *&&prefix, const char *&&msg,
                     const char *&&names, Var... vars) {
    std::cerr << prefix << ' ' << std::quoted(msg) << ": ";

    // Parse variable names
    std::vector<std::string> argNames;
    std::string acc;
    for (; *names; names++) {
        if (*names == ' ')
            continue;
        if (*names == ',') {
            argNames.push_back(acc);
            acc = {};
            continue;
        }
        acc += *names;
    }
    argNames.push_back(acc);

    varsNames(0, argNames, vars...);
}

void varsNamesPrefix(const char *&&prefix, const char *&&msg);
} // namespace __debug

#else
#define db_print(...)
#define db_msg(msg, ...)
#define db_line(msg, ...)
#define db_file
#endif

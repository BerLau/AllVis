#pragma once
#ifndef CORE_TOOLS_H
#define CORE_TOOLS_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

namespace Core::Tools
{
    std::vector<std::string> split_str(const std::string &str, const std::string &delim);
    std::string join_str(const std::vector<std::string> &strs, const std::string &delim);
    std::string fix_length_str(const std::string &str, unsigned int length);
    void reverse_str(char *str, unsigned int length);
}
#endif // CORE_TOOLS_H
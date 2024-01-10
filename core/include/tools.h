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
    void reverse_str(std::string &str);
    void str_replace(std::string &str, const std::string &from, const std::string &to);
    char* str_replace(char* str, char* from, char* to);

    template <typename T, typename Comparator>
    unsigned int binary_search(const T *arr, unsigned int size, const T &target)
    {
        unsigned int l = 0;
        unsigned int r = size - 1;
        while (l <= r)
        {
            unsigned int mid = (l + r) / 2;
            if (Comparator()(arr[mid], target))
            {
                l = mid + 1;
            }
            else if (Comparator()(target, arr[mid]))
            {
                r = mid - 1;
            }
            else
            {
                return mid;
            }
        }
        return -1;
    }
}
#endif // CORE_TOOLS_H
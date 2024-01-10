#include "tools.h"
#include <cstring>
namespace Core::Tools
{
    std::vector<std::string> split_str(const std::string &str, const std::string &delim)
    {
        std::vector<std::string> result;
        std::string::size_type pos1, pos2;
        pos2 = str.find(delim);
        pos1 = 0;
        while (std::string::npos != pos2)
        {
            result.push_back(str.substr(pos1, pos2 - pos1));
            pos1 = pos2 + delim.size();
            pos2 = str.find(delim, pos1);
        }
        if (pos1 != str.length())
            result.push_back(str.substr(pos1));
        else
            result.push_back("");
        return result;
    }

    std::string join_str(const std::vector<std::string> &strs, const std::string &delim)
    {
        std::string result;
        for (auto &str : strs)
        {
            result += str + delim;
        }
        // remove last delim
        result.pop_back();
        return result;
    }

    std::string fix_length_str(const std::string &str, unsigned int length)
    {
        std::string result = str;
        if (result.length() > length)
        {
            result = "..." + result.substr(result.length() - length + 3);
        }
        else
        {
            while (result.length() < length)
            {
                result += " ";
            }
        }
        return result;
    }

    void reverse_str(std::string &str)
    {
        unsigned int l = 0;
        unsigned int r = str.length() -1;
        while (l < r)
        {
            str[l] ^= str[r];
            str[r] ^= str[l];
            str[l] ^= str[r];
            ++l;
            --r;
        }
    }

    void str_replace(std::string &str, const std::string &from, const std::string &to)
    {
        std::string::size_type pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos)
        {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }

    char* str_replace(char *str, char *from, char *to)
    {
        int i = 0;
        int count = 0;

        int from_len = strlen(from);
        int to_len = strlen(to);

        for (i = 0; str[i] != '\0'; i++)
        {
            if (strstr(&str[i], from) == &str[i])
            {
                count++;
                i += from_len - 1;
            }
        }

        char *new_str = (char *)malloc(i + count * (to_len - from_len) + 1);

        i = 0;
        while (*str)
        {
            if (strstr(str, from) == str)
            {
                strcpy(&new_str[i], to);
                i += to_len;
                str += from_len;
            }
            else
            {
                new_str[i++] = *str++;
            }
        }

        new_str[i] = '\0';
        return new_str;
    }

}; // namespace Tools
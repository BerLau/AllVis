#include "tools.h"

namespace Tools
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

}; // namespace Tools
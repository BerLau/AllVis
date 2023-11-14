#include "file.h"
#include <filesystem>

namespace Core
{
    std::vector<std::string> file_list(const std::string &directory, const std::string &extension)
    {
        std::vector<std::string> files;
        for (auto &p : std::filesystem::directory_iterator(directory))
        {
            if (p.path().extension() == extension)
            {
                files.push_back(p.path().string());
            }
        }
        return files;
    }
    std::string file_extension(const std::string &file_name)
    {
        return std::filesystem::path(file_name).extension().string();
    }
}; // namespace Core
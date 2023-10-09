#include "file.hpp"
#include <filesystem>

namespace Core
{
    std::vector<std::string> file_list(const std::string& directory, const std::string& extension)
    {
        std::vector<std::string> files;
        for(auto& p: std::filesystem::directory_iterator(directory)){
            if(p.path().extension() == extension){
                files.push_back(p.path().string());
            }
        }
        return files;
    }
}; // namespace Core
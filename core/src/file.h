#pragma once
#ifndef CORE_FILE_H
#define CORE_FILE_H

#include <string>
#include <vector>

namespace Core
{
    std::vector<std::string> file_list(const std::string& directory, const std::string& extension);
    std::string file_extension(const std::string& file_name);
}; // namespace Core

#endif // CORE_FILE_H
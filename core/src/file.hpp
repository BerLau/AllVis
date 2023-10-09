#pragma once
#ifndef CORE_FILE_H
#define CORE_FILE_H

#include <string>
#include <vector>

namespace Core
{
    std::vector<std::string> file_list(const std::string& directory, const std::string& extension);
}; // namespace Core

#endif // CORE_FILE_H
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <sstream>

namespace e
{
    namespace Utils
    {
        std::string ReadFile(const std::string& filepath);
    }
}

#endif // UTILS_H
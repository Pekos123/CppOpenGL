#include <Utils.h>
#include <iostream>

namespace e
{
    namespace Utils
    {
        std::string ReadFile(const std::string& path)
        {
            std::ifstream file(path);
            if (!file)
                std::cerr << "Failed to open file: " << path;
        
            std::ostringstream ss;
            ss << file.rdbuf();
            return ss.str();
        }
    }
}

#ifndef SHA256_H
#define SHA256_H

#include <string>

class SHA256 {
public:
    static std::string hash(const std::string& data);
    static std::string hashFile(const std::string& filepath);
};

#endif // SHA256_H

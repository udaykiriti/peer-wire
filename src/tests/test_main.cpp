#include "../common/sha256.h"
#include <iostream>
#include <cassert>
#include <string>

void testSHA256() {
    std::cout << "Testing SHA256..." << std::endl;
    // Known test vector
    // "hello world" -> b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9
    std::string input = "hello world";
    std::string expected = "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9";
    
    std::string actual = SHA256::hash(input);
    assert(actual == expected);
    std::cout << "SHA256 'hello world' passed." << std::endl;
    
    std::string empty = "";
    // Empty string hash: e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
    std::string expectedEmpty = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
    assert(SHA256::hash(empty) == expectedEmpty);
    std::cout << "SHA256 empty string passed." << std::endl;
}

int main() {
    testSHA256();
    std::cout << "All unit tests passed." << std::endl;
    return 0;
}

#pragma once
#include <string>
#include <unordered_map>

class Hashing
{
public:
    std::size_t hashing(std::string input_pas, std::string input_salt) {

        std::string sait_pas = input_pas + input_salt;

        std::hash<std::string> hasher;

        std::size_t output_hash = hasher(sait_pas);

        return output_hash;
    };
};

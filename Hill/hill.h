#ifndef HILL_H
#define HILL_H

#include <string>
#include <vector>

#if defined(_WIN32)
    #if defined(HILL_EXPORTS)
        #define HILL_API __declspec(dllexport)
    #else
        #define HILL_API __declspec(dllimport)
    #endif
#else
    #define HILL_API
#endif

extern "C" {
    HILL_API void process_data(std::vector<unsigned char>& data, const std::string& key, bool encrypt);
    HILL_API void encrypt_file(const std::string& input_path, const std::string& output_path, const std::string& key);
    HILL_API void decrypt_file(const std::string& input_path, const std::string& output_path, const std::string& key);
}

#endif // HILL_H
#ifndef VIGINER_H
#define VIGINER_H

#include <string>
#include <vector>

#if defined(_WIN32)
    #if defined(VIGINER_EXPORTS)
        #define VIGINER_API __declspec(dllexport)
    #else
        #define VIGINER_API __declspec(dllimport)
    #endif
#else
    #define VIGINER_API
#endif

extern "C" {
    VIGINER_API void process_data(std::vector<unsigned char>& data, const std::string& key, bool encrypt);
    VIGINER_API void encrypt_file(const std::string& input_path, const std::string& output_path, const std::string& key);
    VIGINER_API void decrypt_file(const std::string& input_path, const std::string& output_path, const std::string& key);
}

#endif // VIGINER_H
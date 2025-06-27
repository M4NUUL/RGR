#ifndef GRONSFELD_H
#define GRONSFELD_H

#include <string>
#include <vector>

#if defined(_WIN32)
    #if defined(GRONSFELD_EXPORTS)
        #define GRONSFELD_API __declspec(dllexport)
    #else
        #define GRONSFELD_API __declspec(dllimport)
    #endif
#else
    #define GRONSFELD_API
#endif

extern "C" {
    GRONSFELD_API void process_data(std::vector<unsigned char>& data, const std::string& key, bool encrypt);
    GRONSFELD_API void encrypt_file(const std::string& input_path, const std::string& output_path, const std::string& key);
    GRONSFELD_API void decrypt_file(const std::string& input_path, const std::string& output_path, const std::string& key);
}

#endif // GRONSFELD_H

#pragma once

#include <stdexcept>

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C"
#endif

// Шифр Гронсфельда

std::string gronsfeldEncrypt(const std::string& text, const std::string& key); // Шифрование

std::string gronsfildDecrypt(const std::string& text, const std::string& key); // Дешифрование
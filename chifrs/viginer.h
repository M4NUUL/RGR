#pragma once

#include <stdexcept>

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C"
#endif

//Шифр Виженера
std::string viginerEncrypt(const std::string &text, const std::string &key); // Шифрока 
std::string viginerDecrypt(const std::string &text, const std::string &key); // Дешифровка

#pragma once

#include <stdexcept>


//Шифр Виженера
std::string viginerEncrypt(const std::string &text, const std::string &key); // Шифрока 
std::string viginerDecrypt(const std::string &text, const std::string &key); // Дешифровка

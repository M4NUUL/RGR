#include "viginer.h"
#include <cctype>
#include <stdexcept>

// Шифровка Вижинера
std::string viginerEncrypt(const std::string &text, const std::string &key)
{
   if (key.empty())
   {
      throw std::invalid_argument("Ключ не может быть пустым");
   }

   std::string result;
   size_t key_length = key.length();
   for (size_t i = 0; i < text.length(); ++i)
   {
      unsigned char current_char = static_cast<unsigned char>(text[i]);
      unsigned char key_char = static_cast<unsigned char>(key[i % key_length]);
      result += static_cast<char>((current_char + key_char) % 256);
   }

   return result;
}

// Дешифровка Вижинера
std::string viginerDecrypt(const std::string text, const std::string &key)
{
   if (key.empty()) throw std::invalid_argument("Ключ не может быть пустым");

   std::string result;
   size_t key_lenght = key.length();

   for (size_t i = 0; i < text.length(); ++i)
   {
      unsigned char current_char = static_cast<unsigned char>(text[i]);
      unsigned char key_char = static_cast<unsigned char>(key[i % key_lenght]);
      result += static_cast<char>((current_char - key_char + 256) % 256);
   }
}

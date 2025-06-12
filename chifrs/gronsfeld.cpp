#include "gronsfeld.h"
#include <stdexcept>
#include <cctype>

// Шифр Гронсфельда

//Шифрование Гронсфельда
std::string gronsfeldEncrypt(const std::string& text, const std::string& key)
{
   if (key.empty())
   {
      throw std::invalid_argument("Ключ не может быть пустым");
   }

   std::string result;
   size_t key_length = key.length();

   for (size_t i = 0; i < text.length(); ++i)
   {
      char key_element = key[i % key_length];
      if (!isdigit(key_element))
      {
         throw std::invalid_argument("Ключ должен содержать только цифры");
      }
      unsigned char current_char = static_cast<unsigned char>(text[i]);
      int shift = key_element - '0';

      current_char = (current_char + shift) % 256;
      result += current_char;
   }

   return result;
}

// Дешифрование Гронсфельда
std::string gronsfildDecrypt(const std::string& text, const std::string& key)
{
   if (key.empty())
   {
      throw std::invalid_argument("Ключ не может быть пустым");
   }

   std::string result;
   size_t key_length = key.length();

   for (size_t i = 0; i < text.length(); ++i)
   {
      char key_element = key[i % key_length];
      if (!isdigit(key_element))
      {
         throw std::invalid_argument("Ключ должен содержать только цифры");
      }
      unsigned char current_char = static_cast<unsigned char>(text[i]);
      int shift = key_element - '0';

      current_char = static_cast<unsigned char>((current_char - shift + 256) % 256);
      result += current_char;
   }

   return result;
}




#include <iostream>
#include <fstream>
#include <cctype>
#include "viginer.h"

void process_data(std::vector<unsigned char> &data, const std::string &key, bool encrypt)
{
   if (key.empty())
   {
      std::cerr << "Ошибка: Ключ не может быть пустым." << std::endl;
      return;
   }

   std::string normalized_key;
   for (char c : key)
   {
      if (std::isalpha(c))
      {
         normalized_key += std::toupper(c);
      }
      else
      {
         std::cerr << "Ошибка: Ключ должен состоять только из букв." << std::endl;
         return;
      }
   }
   if (normalized_key.empty())
   {
      return;
   }

   size_t key_length = normalized_key.size();
   for (size_t i = 0; i < data.size(); ++i)
   {
      char key_char = normalized_key[i % key_length];
      int shift = key_char - 'A';
      if (!encrypt)
      {
         shift = -shift;
      }
      data[i] = static_cast<unsigned char>((data[i] + shift + 256) % 256);
   }
}

void encrypt_file(const std::string &input_path, const std::string &output_path, const std::string &key)
{
   std::ifstream input_file(input_path, std::ios::binary);
   if (!input_file)
   {
      std::cerr << "Ошибка: Не удалось открыть входной файл: " << input_path << std::endl;
      return;
   }
   std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input_file), {});
   input_file.close();

   process_data(buffer, key, true);

   std::ofstream output_file(output_path, std::ios::binary);
   if (!output_file)
   {
      std::cerr << "Ошибка: Не удалось создать выходной файл: " << output_path << std::endl;
      return;
   }
   output_file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
   output_file.close();
}

void decrypt_file(const std::string &input_path, const std::string &output_path, const std::string &key)
{
   std::ifstream input_file(input_path, std::ios::binary);
   if (!input_file)
   {
      std::cerr << "Ошибка: Не удалось открыть входной файл: " << input_path << std::endl;
      return;
   }
   std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input_file), {});
   input_file.close();

   process_data(buffer, key, false);

   std::ofstream output_file(output_path, std::ios::binary);
   if (!output_file)
   {
      std::cerr << "Ошибка: Не удалось создать выходной файл: " << output_path << std::endl;
      return;
   }
   output_file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
   output_file.close();
}
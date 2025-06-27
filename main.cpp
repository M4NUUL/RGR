#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <sstream>

#include "temp.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using GronsfiledProcessDataFunc = void (*)(std::vector<unsigned char> &, const std::string &, bool);
using GronsfiledFileFunc = void (*)(const std::string &, const std::string &, const std::string &);
using ViginerProcessDataFunc = void (*)(std::vector<unsigned char> &, const std::string &, bool);
using ViginerFileFunc = void (*)(const std::string &, const std::string &, const std::string &);
using HillProcessDataFunc = void (*)(std::vector<unsigned char> &, const std::string &, bool);
using HillFileFunc = void (*)(const std::string &, const std::string &, const std::string &);

//=====Гронсфельд=====
void gronsfild_menu()
{
   void *handle = nullptr;
   GronsfiledProcessDataFunc gronsfild_process_data_ptr = nullptr;
   GronsfiledFileFunc gronsfild_encrypt_file_ptr = nullptr, gronsfild_decrypt_file_ptr = nullptr;
#ifdef _WIN32
   handle = LoadLibraryA("gronsfeld.dll");
#else
   handle = dlopen("./libgronsfeld.so", RTLD_LAZY);
#endif
   if (!handle)
   {
      std::cerr << u8"Ошибка: Не удалось загрузить библиотеку Гронсфельда.\n";
      return;
   }

#ifdef _WIN32
   gronsfild_process_data_ptr = (GronsfiledProcessDataFunc)GetProcAddress((HMODULE)handle, "process_data");
   gronsfild_encrypt_file_ptr = (GronsfiledFileFunc)GetProcAddress((HMODULE)handle, "encrypt_file");
   gronsfild_decrypt_file_ptr = (GronsfiledFileFunc)GetProcAddress((HMODULE)handle, "decrypt_file");
#else
   gronsfild_process_data_ptr = (GronsfiledProcessDataFunc)dlsym(handle, "process_data");
   gronsfild_encrypt_file_ptr = (GronsfiledFileFunc)dlsym(handle, "encrypt_file");
   gronsfild_decrypt_file_ptr = (GronsfiledFileFunc)dlsym(handle, "decrypt_file");
#endif

   if (!gronsfild_process_data_ptr || !gronsfild_encrypt_file_ptr || !gronsfild_decrypt_file_ptr)
   {
      std::cerr << u8"Ошибка: Не удалось найти функцию(и) в библиотеке.\n";

#ifdef _WIN32
      FreeLibrary((HMODULE)handle);
#else
      dlclose(handle);
#endif
      return;
   }

   std::cout << u8"\n>>>>шифр Гронсфельда<<<<\n";
   std::cout << u8"1. Шифрование файла\n";
   std::cout << u8"2. Дешифрование файла\n";
   std::cout << u8"3. Шифрование текста\n";
   std::cout << u8"4. Дешифрование текста\n";
   std::cout << u8"0. Назад в главное меню\n";

   int choice = menu_choice(0, 4);
   if (choice == 0)
   {
#ifdef _WIN32
      FreeLibrary((HMODULE)handle);
#else
      dlclose(handle);
#endif
      return;
   }

   std::string in_path, out_path, text, key, result;

   switch (choice)
   {
   case 1: // Шифрование файла
      std::cout << u8"Введите путь к исходному файлу: ";
      std::getline(std::cin, in_path);
      std::cout << u8"Введите путь к целевому файлу: ";
      std::getline(std::cin, out_path);
      std::cout << u8"Введите числовой ключ: ";
      std::getline(std::cin, key);
      try
      {
         gronsfild_encrypt_file_ptr(in_path, out_path, key);
         std::cout << u8"Операция с файлом завершена.\n";
      }
      catch (const std::exception &e)
      {
         std::cerr << u8"Ошибка при работе с файлом: " << e.what() << std::endl;
      }
      break;
   case 2: // Дешифрование файла
      std::cout << u8"Введите путь к исходному файлу: ";
      std::getline(std::cin, in_path);
      std::cout << u8"Введите путь к целевому файлу: ";
      std::getline(std::cin, out_path);
      std::cout << u8"Введите числовой ключ: ";
      std::getline(std::cin, key);
      try
      {
         gronsfild_decrypt_file_ptr(in_path, out_path, key);
         std::cout << u8"Операция с файлом завершена.\n";
      }
      catch (const std::exception &e)
      {
         std::cerr << u8"Ошибка при работе с файлом: " << e.what() << std::endl;
      }
      break;
   case 3: // Шифрование текста
      std::cout << u8"Введите текст для обработки: ";
      std::getline(std::cin, text);
      std::cout << u8"Введите числовой ключ: ";
      std::getline(std::cin, key);
      {
        std::vector<unsigned char> data(text.begin(), text.end());
        gronsfild_process_data_ptr(data, key, true);
        result = std::string(data.begin(), data.end());
        std::cout << u8"Результат: " << result << std::endl;
        std::cout << u8"HEX: ";
        for (unsigned char c : data) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        }
        std::cout << std::dec << std::endl;
      }
      break;
   case 4: // Дешифрование текста
      {
         std::vector<unsigned char> data;
         std::cout << u8"Выберите способ ввода зашифрованного текста:\n";
         std::cout << u8"1. Обычный текст\n";
         std::cout << u8"2. HEX-строка\n";
         int input_type = menu_choice(1, 2);

         if (input_type == 1) {
            std::cout << u8"Введите зашифрованный текст для обработки: ";
            std::getline(std::cin, text);
            data.assign(text.begin(), text.end());
         } else {
            std::cout << u8"Введите зашифрованный текст в HEX-формате: ";
            std::getline(std::cin, text);
            data.clear();
            for (size_t i = 0; i + 1 < text.size(); i += 2) {
                std::string byteStr = text.substr(i, 2);
                unsigned char byte = (unsigned char)std::stoi(byteStr, nullptr, 16);
                data.push_back(byte);
            }
         }
         std::cout << u8"Введите ключ: ";
         std::getline(std::cin, key);
         gronsfild_process_data_ptr(data, key, false); 
         result = std::string(data.begin(), data.end());
         std::cout << u8"Результат: " << result << std::endl;
      }
      break;
   }

#ifdef _WIN32
   FreeLibrary((HMODULE)handle);
#else
   dlclose(handle);
#endif
}

//=====Вижинер=====
void viginer_menu()
{
   void *handle = nullptr;
   ViginerProcessDataFunc viginer_process_data_ptr = nullptr;
   ViginerFileFunc viginer_encrypt_file_ptr = nullptr, viginer_decrypt_file_ptr = nullptr;
#ifdef _WIN32
   handle = LoadLibraryA("viginer.dll");
#else
   handle = dlopen("./libviginer.so", RTLD_LAZY);
#endif
   if (!handle)
   {
      std::cerr << u8"Ошибка: Не удалось загрузить библиотеку Вижинера.\n";
      return;
   }

#ifdef _WIN32
   viginer_process_data_ptr = (ViginerProcessDataFunc)GetProcAddress((HMODULE)handle, "process_data");
   viginer_encrypt_file_ptr = (ViginerFileFunc)GetProcAddress((HMODULE)handle, "encrypt_file");
   viginer_decrypt_file_ptr = (ViginerFileFunc)GetProcAddress((HMODULE)handle, "decrypt_file");
#else
   viginer_process_data_ptr = (ViginerProcessDataFunc)dlsym(handle, "process_data");
   viginer_encrypt_file_ptr = (ViginerFileFunc)dlsym(handle, "encrypt_file");
   viginer_decrypt_file_ptr = (ViginerFileFunc)dlsym(handle, "decrypt_file");
#endif

   if (!viginer_process_data_ptr || !viginer_encrypt_file_ptr || !viginer_decrypt_file_ptr)
   {
      std::cerr << u8"Ошибка: Не удалось найти функцию(и) в библиотеке.\n";

#ifdef _WIN32
      FreeLibrary((HMODULE)handle);
#else
      dlclose(handle);
#endif
      return;
   }

   std::cout << u8"\n>>>>шифр Вижинера<<<<\n";
   std::cout << u8"1. Шифрование файла\n";
   std::cout << u8"2. Дешифрование файла\n";
   std::cout << u8"3. Шифрование текста\n";
   std::cout << u8"4. Дешифрование текста\n";
   std::cout << u8"0. Назад в главное меню\n";

   int choice = menu_choice(0, 4);
   if (choice == 0)
   {
#ifdef _WIN32
      FreeLibrary((HMODULE)handle);
#else
      dlclose(handle);
#endif
      return;
   }

   std::string in_path, out_path, text, key, result;

   switch (choice) {
case 1:
    std::cout << u8"Введите путь к исходному файлу: ";
    std::getline(std::cin, in_path);
    std::cout << u8"Введите путь к целевому файлу: ";
    std::getline(std::cin, out_path);
    std::cout << u8"Введите ключ (буквы): ";
    std::getline(std::cin, key);
    try {
        viginer_encrypt_file_ptr(in_path, out_path, key);
        std::cout << u8"Операция с файлом завершена.\n";
    } catch (const std::exception &e) {
        std::cerr << u8"Ошибка при работе с файлом: " << e.what() << std::endl;
    }
    break;
case 2:
    std::cout << u8"Введите путь к исходному файлу: ";
    std::getline(std::cin, in_path);
    std::cout << u8"Введите путь к целевому файлу: ";
    std::getline(std::cin, out_path);
    std::cout << u8"Введите ключ (буквы): ";
    std::getline(std::cin, key);
    try {
        viginer_decrypt_file_ptr(in_path, out_path, key);
        std::cout << u8"Операция с файлом завершена.\n";
    } catch (const std::exception &e) {
        std::cerr << u8"Ошибка при работе с файлом: " << e.what() << std::endl;
    }
    break;
case 3:
    std::cout << u8"Введите текст для обработки: ";
    std::getline(std::cin, text);
    std::cout << u8"Введите ключ (буквы): ";
    std::getline(std::cin, key);
    {
        std::vector<unsigned char> data(text.begin(), text.end());
        viginer_process_data_ptr(data, key, true);
        result = std::string(data.begin(), data.end());
        std::cout << u8"Результат: " << result << std::endl;
        std::cout << u8"HEX: ";
        for (unsigned char c : data) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        }
        std::cout << std::dec << std::endl;
    }
    break;
case 4:
    {
        std::vector<unsigned char> data;
        std::cout << u8"Выберите способ ввода зашифрованного текста:\n";
        std::cout << u8"1. Обычный текст\n";
        std::cout << u8"2. HEX-строка\n";
        int input_type = menu_choice(1, 2);

        if (input_type == 1) {
            std::cout << u8"Введите зашифрованный текст для обработки: ";
            std::getline(std::cin, text);
            data.assign(text.begin(), text.end());
        } else {
            std::cout << u8"Введите зашифрованный текст в HEX-формате: ";
            std::getline(std::cin, text);
            data.clear();
            for (size_t i = 0; i + 1 < text.size(); i += 2) {
                std::string byteStr = text.substr(i, 2);
                unsigned char byte = (unsigned char)std::stoi(byteStr, nullptr, 16);
                data.push_back(byte);
            }
        }
        std::cout << u8"Введите ключ (буквы): ";
        std::getline(std::cin, key);
        viginer_process_data_ptr(data, key, false);
        result = std::string(data.begin(), data.end());
        std::cout << u8"Результат: " << result << std::endl;
    }
    break;
}

#ifdef _WIN32
   FreeLibrary((HMODULE)handle);
#else
   dlclose(handle);
#endif
}

//=====Шифр Хилла=====
void hill_menu()
{
    void *handle = nullptr;
    HillProcessDataFunc hill_process_data_ptr = nullptr;
    HillFileFunc hill_encrypt_file_ptr = nullptr, hill_decrypt_file_ptr = nullptr;
#ifdef _WIN32
    handle = LoadLibraryA("hill.dll");
#else
    handle = dlopen("./libhill.so", RTLD_LAZY);
#endif
    if (!handle)
    {
        std::cerr << u8"Ошибка: Не удалось загрузить библиотеку Хилла.\n";
        return;
    }

#ifdef _WIN32
    hill_process_data_ptr = (HillProcessDataFunc)GetProcAddress((HMODULE)handle, "process_data");
    hill_encrypt_file_ptr = (HillFileFunc)GetProcAddress((HMODULE)handle, "encrypt_file");
    hill_decrypt_file_ptr = (HillFileFunc)GetProcAddress((HMODULE)handle, "decrypt_file");
#else
    hill_process_data_ptr = (HillProcessDataFunc)dlsym(handle, "process_data");
    hill_encrypt_file_ptr = (HillFileFunc)dlsym(handle, "encrypt_file");
    hill_decrypt_file_ptr = (HillFileFunc)dlsym(handle, "decrypt_file");
#endif

    if (!hill_process_data_ptr || !hill_encrypt_file_ptr || !hill_decrypt_file_ptr)
    {
        std::cerr << u8"Ошибка: Не удалось найти функцию(и) в библиотеке.\n";
#ifdef _WIN32
        FreeLibrary((HMODULE)handle);
#else
        dlclose(handle);
#endif
        return;
    }

    std::cout << u8"\n>>>>шифр Хилла<<<<\n";
    std::cout << u8"1. Шифрование файла\n";
    std::cout << u8"2. Дешифрование файла\n";
    std::cout << u8"3. Шифрование текста\n";
    std::cout << u8"4. Дешифрование текста\n";
    std::cout << u8"0. Назад в главное меню\n";

    int choice = menu_choice(0, 4);
    if (choice == 0)
    {
#ifdef _WIN32
        FreeLibrary((HMODULE)handle);
#else
        dlclose(handle);
#endif
        return;
    }

    std::string in_path, out_path, text, key, result;

    switch (choice)
    {
    case 1: // Шифрование файла
        std::cout << u8"Введите путь к исходному файлу: ";
        std::getline(std::cin, in_path);
        std::cout << u8"Введите путь к целевому файлу: ";
        std::getline(std::cin, out_path);
        std::cout << u8"Введите ключ (строка чисел): ";
        std::getline(std::cin, key);
        try
        {
            hill_encrypt_file_ptr(in_path, out_path, key);
            std::cout << u8"Операция с файлом завершена.\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << u8"Ошибка при работе с файлом: " << e.what() << std::endl;
        }
        break;
    case 2: // Дешифрование файла
        std::cout << u8"Введите путь к исходному файлу: ";
        std::getline(std::cin, in_path);
        std::cout << u8"Введите путь к целевому файлу: ";
        std::getline(std::cin, out_path);
        std::cout << u8"Введите ключ (строка чисел): ";
        std::getline(std::cin, key);
        try
        {
            hill_decrypt_file_ptr(in_path, out_path, key);
            std::cout << u8"Операция с файлом завершена.\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << u8"Ошибка при работе с файлом: " << e.what() << std::endl;
        }
        break;
    case 3: // Шифрование текста
        std::cout << u8"Введите текст для обработки: ";
        std::getline(std::cin, text);
        std::cout << u8"Введите ключ (строка чисел): ";
        std::getline(std::cin, key);
        {
            std::vector<unsigned char> data(text.begin(), text.end());
            hill_process_data_ptr(data, key, true);
            result = std::string(data.begin(), data.end());
            std::cout << u8"Результат: " << result << std::endl;
            std::cout << u8"HEX: ";
            for (unsigned char c : data) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
            }
            std::cout << std::dec << std::endl;
        }
        break;
    case 4: // Дешифрование текста
        {
         std::vector<unsigned char> data;
         std::cout << u8"Выберите способ ввода зашифрованного текста:\n";
         std::cout << u8"1. Обычный текст\n";
         std::cout << u8"2. HEX-строка\n";
         int input_type = menu_choice(1, 2);

         if (input_type == 1) {
            std::cout << u8"Введите зашифрованный текст для обработки: ";
            std::getline(std::cin, text);
            data.assign(text.begin(), text.end());
         } else {
            std::cout << u8"Введите зашифрованный текст в HEX-формате: ";
            std::getline(std::cin, text);
            data.clear();
            for (size_t i = 0; i + 1 < text.size(); i += 2) {
                std::string byteStr = text.substr(i, 2);
                unsigned char byte = (unsigned char)std::stoi(byteStr, nullptr, 16);
                data.push_back(byte);
            }
         }
         std::cout << u8"Введите ключ (строка чисел): ";
         std::getline(std::cin, key);
         hill_process_data_ptr(data, key, false);
         result = std::string(data.begin(), data.end());
         std::cout << u8"Результат: " << result << std::endl;
      }
      break;
    }

#ifdef _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}

int main()
{
   setup_windows_console();
   printShieldWithTitle();
   std::cout << u8"Добро пожаловать в программу шифрования и дешифрования!\n";

   while (true)
   {
      std::cout << u8"=====EN-DEcrypt=====\n";
      std::cout << u8"Способ шифрования:\n";
      std::cout << u8"1. Гронсфельд\n";
      std::cout << u8"2. Вижинер\n";
      std::cout << u8"3. Хилл\n";
      std::cout << u8"0. Выход\n";

      int choice = menu_choice(0, 3);
      switch (choice)
      {
      case 0:
         std::cout << u8"Выход из программы.\n";
         return 0;
      case 1:
         gronsfild_menu();
         break;
      case 2:
         viginer_menu();
         break;
      case 3:
         hill_menu();
         break;
      default:
         std::cout << u8"Неверный выбор.\n";
         break;
      }
   }
   return 0;
}
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>
#include <fstream>
#include <sstream>

#include "temp.h"
#include "viginer.h"
#include "hill.h"
#include "gronsfeld.h"


int main()
{
   try
   {
      // Стартовая настройка консоли
      SetConsoleOutputCP(CP_UTF8); // Для вывода
      SetConsoleCP(CP_UTF8);       // Для ввода
      printShieldWithTitle();

      // Защита

      int attempts = 3;
      std::string password;
      bool access_password = false;

      while (attempts-- > 0)
      {
         std::cout << "Введите пароль для доступа к программе: ";
         std::cin >> password;

         if (password == "password")
         {
            access_password = true;
            break;
         }

         else
         {
            std::cout << "Неверный пароль. Осталось попыток: " << attempts << "\n";
         }
      }

      if (!access_password)
      {
         std::cout << "Доступ запрещен. Программа будет закрыта.\n";
         pauseAndExit();
         return 1;
      }

      std::cout << "Добро пожаловать.";

      // Выбор ввода текста
      int chouce;
      std::string input;
      std::cout << "\nВыберите способ ввода текста:\n";
      std::cout << "1. Ввести текст вручную\n";
      std::cout << "2. Считать текст из файла\n";
      std::cout << "Введите номер способа: ";

      if (!safeInput(chouce, "Ошибка, введите число 1 или 2"))
      {
         pauseAndExit();
         return 1;
      }

      if (chouce != 1 && chouce != 2)
      {
         std::cerr << "Ошибка: введите число 1 или 2.\n";
         pauseAndExit();
         return 1;
      }

      else if (chouce == 1)
      {
         std::cout << "Введите текст вручную: ";
         std::getline(std::cin, input);
      }

      else
      {
         std::string filename;
         std::cout << "Введите имя файла (например: input.txt): ";
         std::getline(std::cin, filename);

         std::ifstream inFile(filename, std::ios::binary);

         if (!inFile)
         {
            std::cerr << "Ошибка: файл не найден.\n";
            pauseAndExit();
            return 1;
         }

         std::ostringstream ss;
         ss << inFile.rdbuf();
         input = ss.str();
         inFile.close();
         std::cout << "Файл успешно прочитан.\n";
      }

      // Выбор Шифрования
      int choice_chifrs;
      CiperTipe cipher;
      bool valid_choice = false;
      
      while (!valid_choice)
      {
         std::cout << "Выберите тип шифрования:\n";
         
         std::cout << "1. Шифр Хилла\n";
         std::cout << "2. Шифр Гронсфельда\n";
         std::cout << "3. Шифр Виженера\n";
         std::cout << "Введите номер шифра: ";
      }

       
   
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << '\n';
   }
   return 0;
}
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>
#include <fstream>
#include <sstream>

#include "temp.h"

// Анимация процесса обработки
void simulateProcessing()
{
   std::cout << "\n[+] Процесс шифрования";
   for (int i = 0; i < 3; ++i)
   {
      std::cout << ".";
      std::cout.flush();
      std::this_thread::sleep_for(std::chrono::milliseconds(600));
   }
   std::cout << " Готово!\n";
};

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
      bool accessPassword = false;
      while (attempts-- > 0)
      {
         std::cout << "Введите пароль для доступа к программе: ";
         std::cin >> password;
         if (password == "password")
         {
            accessPassword = true;
            break;
         }
         else
         {
            std::cout << "Неверный пароль. Осталось попыток: " << attempts << "\n";
         }
      }
      if (!accessPassword)
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
      if (safeInput(chouce, "Ошибка, введите число 1 или 2"))
      {
         pauseAndExit();
         return 1;
      }
      if (chouce == 1)
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
   }
catch (const std::exception &e)
{
   std::cerr << e.what() << '\n';
}
return 0;
}
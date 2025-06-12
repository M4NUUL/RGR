#include <iostream>
#include <fstream>
#include <thread>
#include <limits>
#include <random>
#include <cctype>
#include <algorithm>

#include "temp.h"


// Заставка
void printShieldWithTitle()
{
   std::cout << "\\_              _/                                                                             \\_              _/      \n";
   std::cout << "] --__________-- [                                                                             ] --__________-- [\n";
   std::cout << "|       ||       | =========================================================================== |       ||       |\n";
   std::cout << "\\       ||       / _____   __   __          ______    _____                                    \\       ||       /\n";
   std::cout << " [      ||      ] | ____| |  \\ |  |        |   __ \\  |  ___|                              _     [      ||      ]\n";
   std::cout << " |______||______| | |__   |   \\|  |  ____  |  |  | \\ | |__     ___   _ __   _   _  _ __  | |_   |______||______|\n";
   std::cout << " |------..------| |  __|  |  |\\   | |____| |  |  | |||  __|   / __| | `__| | | | || `_ \\ |  _|  |------..------|\n";
   std::cout << " ]      ||      [ | |___  |  | \\  |        |  |__| //| |___   | |_  | |    | |_| || |_) )| |_   ]      ||      [\n";
   std::cout << "  \\     ||     /  |_____| |__|  \\_|        |______// |_____|  \\___| |_|     \\__, || ,__/  \\___|  \\     ||     /\n";
   std::cout << "   [    ||    ]                                                             |___/ |_|             [    ||    ]\n";
   std::cout << "   \\    ||    /   =============================================================================   \\    ||    /\n";
   std::cout << "    [   ||   ]                                                                                     [   ||   ]\n";
   std::cout << "     \\__||__/                                                                                       \\__||__/\n";
   std::cout << "        --                                                                                             --\n";
}

// Выход из программы
void pauseAndExit()
{
    std::cin.clear();
    std::cout << "\nНажмите Enter для выхода...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}


// Безопасный ввод числа
bool safeInput(int& number, const std::string& Error)
{
   if(!(std::cin >> number))
   {
      std::cin.clear(); // Очистка флага ошибки
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
      std::cout << Error << std::endl; 
      return false;
   }
   else
   {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
      return true;
   }
}
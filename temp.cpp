#include <iostream>
#include <limits>
#include "temp.h"

#ifdef _WIN32
#include <windows.h>
#endif

void setup_windows_console()
{
#ifdef _WIN32
       SetConsoleCP(CP_UTF8);
   SetConsoleOutputCP(CP_UTF8);
#endif
}

void printShieldWithTitle()
{
   std::cout << "\\_              _/                                                                             \\_              _/      \n";
   std::cout << "] --__________-- [                                                                             ] --__________-- [\n";
   std::cout << "|       ||       | =========================================================================== |       ||       |\n";
   std::cout << "\\       ||       / _____   __   __          ______    _____                                    \\       ||       /\n";
   std::cout << " [      ||      ] | ____| |  \\ |  |        |   __ \\\\  |  ___|                              _     [      ||      ]\n";
   std::cout << " |______||______| | |__   |   \\|  |  ____  |  |  | \\\\ | |__     ___   _ __   _   _  _ __  | |_   |______||______|\n";
   std::cout << " |------..------| |  __|  |  |\\   | |____| |  |  | |||  __|   / __| | `__| | | | || `_ \\ |  _|  |------..------|\n";
   std::cout << " ]      ||      [ | |___  |  | \\  |        |  |__| //| |___   | |_  | |    | |_| || |_) )| |_   ]      ||      [\n";
   std::cout << "  \\     ||     /  |_____| |__|  \\_|        |______// |_____|  \\___| |_|     \\__, || ,__/  \\___|  \\     ||     /\n";
   std::cout << "   [    ||    ]                                                             |___/ |_|             [    ||    ]\n";
   std::cout << "   \\    ||    /   =============================================================================   \\    ||    /\n";
   std::cout << "    [   ||   ]                                                                                     [   ||   ]\n";
   std::cout << "     \\__||__/                                                                                       \\__||__/\n";
   std::cout << "        --                                                                                             --\n";
}

void clear_cin() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int menu_choice(int min, int max) {
    int choice;
    while (true) {
        std::cout << u8">> ";
        std::cin >> choice;
        if (!std::cin || choice < min || choice > max) {
            std::cout << u8"Ошибка: Пожалуйста, введите число между " << min << u8" и " << max << u8".\n";
            std::cin.clear();
            clear_cin();
        } else {
            clear_cin();
            return choice;
        }
    }
}
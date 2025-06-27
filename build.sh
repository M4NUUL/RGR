#!/bin/bash
set -e

ERROR_CODE=0
build_dir="build"

echo "Начало сборки для Linux/macOS..."

# Создаем директорию для сборки, если не существует
mkdir -p "$build_dir"

# ----------- Сборка библиотеки Gronsfeld -----------
echo "Сборка Gronsfeld..."
g++ -fPIC -shared -o "$build_dir/libgronsfeld.so" Gronsfild/gronsfeld.cpp -IGronsfild -std=c++17 -DGRONSFELD_EXPORTS || { echo "Ошибка сборки Gronsfeld."; exit 1; }
echo "Gronsfeld собран."

# ----------- Сборка библиотеки Viginer -----------
echo "Сборка Viginer..."
g++ -fPIC -shared -o "$build_dir/libviginer.so" Viginer/viginer.cpp -IViginer -std=c++17 -DVIGINER_EXPORTS || { echo "Ошибка сборки Viginer."; exit 1; }
echo "Viginer собран."

# ----------- Сборка библиотеки Hill -----------
echo "Сборка Hill..."
g++ -fPIC -shared -o "$build_dir/libhill.so" Hill/hill.cpp -IHill -std=c++17 -DHILL_EXPORTS || { echo "Ошибка сборки Hill."; exit 1; }
echo "Hill собран."

# ----------- Сборка основного исполняемого файла -----------
echo "Сборка основного исполняемого файла..."
g++ -o "$build_dir/EN-DEcrypto" main.cpp temp.cpp \
    -L"$build_dir" -lgronsfeld -lviginer -lhill \
    -std=c++17 || { echo "Ошибка сборки основного исполняемого файла."; exit 1; }
echo "Основной исполняемый файл собран."

echo "Сборка для Linux/macOS завершена. Исполняемый файл и библиотеки находятся в $build_dir/"
exit 0
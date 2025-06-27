@echo off
setlocal enabledelayedexpansion

::Отсановка при первой ошибке
set "ERROR_CODE=0" 
pushd .

::Дириктория библиотек
set "build_dir=build"

echo Начало сборки для Windows...

::Создаем директорию для скомпилированных файлов, если она не существует
if not exist "%build_dir%" mkdir "%build_dir%"

::-------------- Сборка библиотеки Gronsfeld ----------------
echo Сборка Gronsfeld...
g++ -shared -o "%build_dir%\gronsfeld.dll" Gronsfild\gronsfeld.cpp -IGronsfild -std=c++17 -DGRONSFELD_EXPORTS
if %ERRORLEVEL% NEQ 0 (
    echo Ошибка сборки Gronsfeld.
    set "ERROR_CODE=1"
    goto :cleanup
)
echo Gronsfeld собран.

::-------------- Сборка библиотеки Viginer ----------------
echo Сборка Viginer...
g++ -shared -o "%build_dir%\viginer.dll" Viginer\viginer.cpp -IViginer -std=c++17 -DVIGINER_EXPORTS
if %ERRORLEVEL% NEQ 0 (
    echo Ошибка сборки Viginer.
    set "ERROR_CODE=1"
    goto :cleanup
)
echo Viginer собран.

::-------------- Сборка библиотеки Hill ----------------
echo Сборка Hill...
g++ -shared -o "%build_dir%\hill.dll" Hill\hill.cpp -IHill -std=c++17 -DHILL_EXPORTS
if %ERRORLEVEL% NEQ 0 (
    echo Ошибка сборки Hill.
    set "ERROR_CODE=1"
    goto :cleanup
)
echo Hill собран.

::-------------- Сборка исполняемого файла Main ----------------
echo Сборка основного исполняемого файла...
:: Ссылаемся на DLL-файлы. Для MinGW обычно достаточно -L и -l.
g++ -o "%build_dir%\EN-DEcrypto.exe" main.cpp temp.cpp^
    -L"%build_dir%" -lgronsfeld -lviginer -lhill ^
    -std=c++17
if %ERRORLEVEL% NEQ 0 (
    echo Ошибка сборки основного исполняемого файла.
    set "ERROR_CODE=1"
    goto :cleanup
)
echo Основной исполняемый файл собран.

echo Сборка для Windows завершена. Исполняемый файл и библиотеки находятся в %build_dir%\
goto :eof

:cleanup
echo "Произошла ошибка сборки."
pause
exit /b %ERROR_CODE%

:eof
popd
exit /b %ERROR_CODE%
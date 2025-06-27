#include <iostream>
#include <fstream>
#include <cctype>
#include "gronsfeld.h"

void process_data(std::vector<unsigned char> &data, const std::string &key, bool encrypt)
{
    if (key.empty())
    {
        std::cerr << "Ошибка: Ключ не может быть пустым." << std::endl;
        return;
    }

    std::vector<int> numeric_key;
    for (char c : key)
    {
        if (isdigit(c))
        {
            numeric_key.push_back(c - '0');
        }
        else
        {
            std::cerr << "Ошибка: Ключ должен состоять только из цифр." << std::endl;
            return;
        }
    }
    if (numeric_key.empty())
    {
        return;
    }

    for (size_t i = 0; i < data.size(); ++i)
    {
        int shift = numeric_key[i % numeric_key.size()];
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
    output_file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    output_file.close();
    std::cout << "Файл успешно зашифрован: " << output_path << std::endl;
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

    output_file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    output_file.close();

    std::cout << "Файл успешно дешифрован: " << output_path << std::endl;
}

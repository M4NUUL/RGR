#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <numeric>
#include "hill.h"

namespace {
    // Размер матрицы и модуль для операций
    const int N = 5; // Теперь обрабатывается 3 байта за раз. Ключ: 9 целых чисел (матрица 3×3). Эффективное криптопространство: около 2⁷² возможных ключей (по формуле из литературы) Меньше — менее стойко, больше — сложнее найти обратимую матрицу (и дольше работает).
    const int MOD = 256;  // Мы работаем с байтами, а каждый байт — это число от 0 до 255 → Всего 256 значений → Чтобы остаться в этом диапазоне, мы считаем все операции по модулю 256.

    std::vector<std::vector<int>> parse_key(const std::string &key) {
        std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
        std::istringstream stream(key);
        for (int i = 0; i < N * N; ++i) {
            int val;
            if (!(stream >> val)) {
                std::cerr << "Ошибка: ключ должен содержать 9 целых чисел" << std::endl;
                return {};
            }
            matrix[i / N][i % N] = val % MOD;
        }
        return matrix;
    }

    int mod_inv(int a, int m) {
        int m0 = m, t, q;
        int x0 = 0, x1 = 1;
        if (m == 1) return 0;
        while (a > 1) {
            q = a / m;
            t = m;
            m = a % m; a = t;
            t = x0;
            x0 = x1 - q * x0; x1 = t;
        }
        return (x1 + m0) % m0;
    }

    std::vector<std::vector<int>> inverse_matrix(const std::vector<std::vector<int>>& mat) {
        int n = N;
        std::vector<std::vector<int>> a = mat;
        std::vector<std::vector<int>> inv(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; ++i) inv[i][i] = 1;

        for (int i = 0; i < n; ++i) {
            int val = a[i][i];
            int inv_val = mod_inv(val, MOD);
            if (val == 0 || inv_val == 0) {
                std::cerr << "Ошибка: матрица не обратима" << std::endl;
                return {};
            }
            for (int j = 0; j < n; ++j) {
                a[i][j] = (a[i][j] * inv_val) % MOD;
                inv[i][j] = (inv[i][j] * inv_val) % MOD;
            }
            for (int k = 0; k < n; ++k) {
                if (k == i) continue;
                int factor = a[k][i];
                for (int j = 0; j < n; ++j) {
                    a[k][j] = (a[k][j] - factor * a[i][j] % MOD + MOD) % MOD;
                    inv[k][j] = (inv[k][j] - factor * inv[i][j] % MOD + MOD) % MOD;
                }
            }
        }
        return inv;
    }

    void process(std::vector<unsigned char> &data, const std::string &key, bool encrypt) {
        std::vector<std::vector<int>> matrix = parse_key(key);
        if (matrix.empty()) return;
        if (!encrypt) {
            matrix = inverse_matrix(matrix);
            if (matrix.empty()) {
                std::cerr << "Ошибка: Ключ не подходит (матрица не обратима)" << std::endl;
                return;
            }
        }

        while (data.size() % N != 0)
            data.push_back(0);

        for (size_t i = 0; i < data.size(); i += N) {
            std::vector<int> block(N);
            for (int j = 0; j < N; ++j)
                block[j] = data[i + j];
            for (int r = 0; r < N; ++r) {
                int sum = 0;
                for (int c = 0; c < N; ++c)
                    sum += matrix[r][c] * block[c];
                data[i + r] = static_cast<unsigned char>((sum % MOD + MOD) % MOD);
            }
        }
    }
}

void process_data(std::vector<unsigned char> &data, const std::string &key, bool encrypt) {
    process(data, key, encrypt);
}

void encrypt_file(const std::string &input_path, const std::string &output_path, const std::string &key) {
    std::ifstream input_file(input_path, std::ios::binary);
    if (!input_file) {
        std::cerr << "Ошибка: Не удалось открыть входной файл: " << input_path << std::endl;
        return;
    }
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input_file), {});
    input_file.close();

    process_data(buffer, key, true);

    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file) {
        std::cerr << "Ошибка: Не удалось создать выходной файл: " << output_path << std::endl;
        return;
    }
    output_file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    output_file.close();
    std::cout << "Файл успешно зашифрован: " << output_path << std::endl;
}

void decrypt_file(const std::string &input_path, const std::string &output_path, const std::string &key) {
    std::ifstream input_file(input_path, std::ios::binary);
    if (!input_file) {
        std::cerr << "Ошибка: Не удалось открыть входной файл: " << input_path << std::endl;
        return;
    }
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input_file), {});
    input_file.close();

    process_data(buffer, key, false);

    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file) {
        std::cerr << "Ошибка: Не удалось создать выходной файл: " << output_path << std::endl;
        return;
    }
    output_file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    output_file.close();
    std::cout << "Файл успешно дешифрован: " << output_path << std::endl;
}

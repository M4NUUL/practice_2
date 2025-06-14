
#pragma once
#include <string>
#include <tuple>
#include <stdint.h>

uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod);

// Генерирует ключи для RSA алгоритма
std::tuple<uint64_t, uint64_t, uint64_t> rsaGenerateKeys();

// Шифрует текст алгоритмом RSA
std::string rsaEncrypt(const std::string& text, uint64_t e, uint64_t n);

// Дешифрует текст, зашифрованный алгоритмом RSA
std::string rsaDecrypt(const std::string& cipher, uint64_t d, uint64_t n);

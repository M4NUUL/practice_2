#pragma once

#include <vector>

typedef __int128_t i128;

bool isPrime(i128 num);

std::vector<bool> getBinaryExpant(i128 exp);

i128 binaryExponentation(i128 a, i128 exp, i128 p);

i128 theoremFerma(i128 a, i128 x, i128 p);

i128 fastPowerLog(i128 a, i128 x, i128 p);
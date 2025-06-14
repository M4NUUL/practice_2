#include <iostream>
#include <cmath>
#include <vector>

#include "ferm.h"

typedef __int128_t i128;

// Проверка на простоту
bool isPrime(i128 num)
{
   if (num < 2)
      return false;
   else if (num <= 3)
      return true;
   else if (num % 2 == 0 || num % 3 == 0)
      return false;

   i128 new_int = static_cast<i128>(sqrt(num)) + 1;
   for (i128 i = 5; i * i <= num; i += 6)
   {
      if (num % i == 0 || num % (i + 2) == 0)
         return 0;
   }
   return true;
}

std::vector<bool> getBinaryExpant(i128 exp)
{
   std::vector<bool> bits;
   while (exp > 0)
   {
      bits.push_back(exp % 2);
      exp /= 2;
   }
   return bits;
}

i128 binaryExponentation(i128 a, i128 exp, i128 p)
{
   if (exp == 0)
      return 1;

   std::vector<bool> bits = getBinaryExpant(exp);
   i128 result = 1;
   i128 current_power = a % p;

   for (bool bit : bits)
   {
      if (bit)
      {
         result = (result * current_power) % p;
      }
      current_power = (current_power * current_power) % p;
   }
   return result;
}

i128 theoremFerma(i128 a, i128 x, i128 p)
{
   if (a % p == 0)
   {
      std::cout << "Условие теоремы Ферма не выполняется, a(mod p) = 0" << std::endl;
      return 0;
   }

   i128 reduced_exp = x % (p - 1);
   return binaryExponentation(a, reduced_exp, p);
}

// Оптимизированый вариант log2(x) для больших чисел
int bitLength(i128 n)
{
   int length = 0;
   while (n > 0)
   {
      n >>= 1;
      length++;
   }
   return length;
}

// Логарифмическое вычисление a^x mod p
i128 fastPowerLog(i128 a, i128 x, i128 p)
{
   if (p <= 0)
      throw std::invalid_argument("число p должно быть больше 0");
   i128 result = 1;
   a = a % p;
   if (a < 0)
      a += p;

   while (x > 0)
   {
      if (x & 1)
         result = (result * a) % p;
      a = (a * a) % p;
      x >>= 1;
   }
   return result;
}

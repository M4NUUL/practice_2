#include "evclid.h"

typedef __int128_t i128;

i128 extendedGCD(i128 a, i128 b, i128 &u, i128 &v)
{
    if (b == 0)
    {
        u = 1;
        v = 0;
        return a;
    }

    i128 u1, v1;
    i128 d = extendedGCD(b, a % b, u1, v1);

    u = v1;
    v = u1 - (a / b) * v1;

    return d;
}

// Функция для нахождения обратного элемента по модулю m
i128 inverseElement(i128 c, i128 m)
{
   i128 u, v;
   i128 g = extendedGCD(c, m, u, v);

   if (g != 1)
      return -1;
   return (u % m + m) % m;
}
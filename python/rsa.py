from typing import Tuple

def gcd(a: int, b: int) -> int:
    while b != 0:
        a, b = b, a % b
    return a

def mod_inverse(a: int, m: int) -> int:
    m0, x0, x1 = m, 0, 1
    while a > 1:
        q = a // m
        a, m = m, a % m
        x0, x1 = x1 - q * x0, x0
    return x1 + m0 if x1 < 0 else x1

def mod_pow(base: int, exp: int, mod: int) -> int:
    result = 1
    base %= mod
    while exp > 0:
        if exp % 2 == 1:
            result = (result * base) % mod
        base = (base * base) % mod
        exp //= 2
    return result

def rsa_generate_keys() -> Tuple[int, int, int]:
    p, q = 61, 53
    n = p * q
    phi = (p - 1) * (q - 1)
    e = 17
    while gcd(e, phi) != 1:
        e += 1
    d = mod_inverse(e, phi)
    return n, e, d

def rsa_encrypt(text: str, e: int, n: int) -> str:
    return ' '.join(str(mod_pow(ord(ch), e, n)) for ch in text)

def rsa_decrypt(cipher: str, d: int, n: int) -> str:
    return ''.join(chr(mod_pow(int(num), d, n)) for num in cipher.split())

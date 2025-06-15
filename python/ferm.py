def is_prime(num: int) -> bool:
    if num < 2:
        return False
    elif num <= 3:
        return True
    elif num % 2 == 0 or num % 3 == 0:
        return False
    i = 5
    while i * i <= num:
        if num % i == 0 or num % (i + 2) == 0:
            return False
        i += 6
    return True

def get_binary_expansion(exp: int) -> list[bool]:
    bits = []
    while exp > 0:
        bits.append(exp % 2 == 1)
        exp //= 2
    return bits

def binary_exponentiation(a: int, exp: int, p: int) -> int:
    if exp == 0:
        return 1
    bits = get_binary_expansion(exp)
    result = 1
    current_power = a % p
    for bit in bits:
        if bit:
            result = (result * current_power) % p
        current_power = (current_power * current_power) % p
    return result

def theorem_fermat(a: int, x: int, p: int) -> int:
    if a % p == 0:
        print("Условие теоремы Ферма не выполняется, a(mod p) = 0")
        return 0
    reduced_exp = x % (p - 1)
    return binary_exponentiation(a, reduced_exp, p)

def bit_length(n: int) -> int:
    length = 0
    while n > 0:
        n >>= 1
        length += 1
    return length

def fast_power_log(a: int, x: int, p: int) -> int:
    if p <= 0:
        raise ValueError("число p должно быть больше 0")
    result = 1
    a %= p
    if a < 0:
        a += p
    while x > 0:
        if x & 1:
            result = (result * a) % p
        a = (a * a) % p
        x >>= 1
    return result

def extended_gcd(a: int, b: int) -> tuple[int, int, int]:
    if b == 0:
        return a, 1, 0
    d, u1, v1 = extended_gcd(b, a % b)
    u = v1
    v = u1 - (a // b) * v1
    return d, u, v


def inverse_element(c: int, m: int) -> int:
    g, u, _ = extended_gcd(c, m)
    if g != 1:
        return -1
    return (u % m + m) % m  # корректировка на случай отрицательного u

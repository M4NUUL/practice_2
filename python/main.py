import sys
from ferm import *
from evclid import *
from rsa import *

def safe_int_input(prompt, err_msg):
    try:
        return int(input(prompt))
    except ValueError:
        print(err_msg)
        return None

def show_menu():
    print("Выберете задачу для проверки:")
    print("1. Проверка задания №1")
    print("2. Проверка задания №2")
    print("3. Проверка задания №3")
    print("4. Проверка задания №4")
    print("5. Проверка задания №5")
    print("6. Проверка задания №6")
    print("0. Выход")
    return input("Выбор: ")

def task1():
    a = int(input("Введите a: "))
    exp = int(input("Введите x: "))
    p = int(input("Введите p: "))
    if not is_prime(p):
        print("Число не простое, теорема Ферма не применима.\n")
        return
    print("Результат бинарным разложением:", theorem_ferma(a, exp, p))
    print("Результат через быстрое возведение в степень:", fast_power_mod(a, exp, p))
    print()

def task2():
    c = int(input("Введите c: "))
    m = int(input("Введите m: "))
    d = inverse_element(c, m)
    if d == -1:
        print("Решение невозможно, НОД(c, m) != 1")
    else:
        print(f"Решение: d = {d}")
        print(f"Проверка: {c}*{d} mod {m} = {(c*d) % m}")
    print()

def task3():
    c = int(input("Введите c: "))
    m = int(input("Введите m: "))
    inv = inverse_element(c, m)
    if inv == -1:
        print("Обратного элемента не существует")
    else:
        print(f"Обратный элемент: {c}^-1 mod {m} = {inv}")
        print(f"Проверка: {c} * {inv} mod {m} = {(c*inv) % m}")
    print()

def task4():
    fname = input("Введите имя бинарного файла (например, image.png): ")
    try:
        data = open(fname, "rb").read()
        print(f"Файл успешно прочитан ({len(data)} байт).")
    except IOError:
        print("Ошибка: файл не найден.")
        print()
        return

    action = safe_int_input("1. Зашифровать\n2. Дешифровать\nВаш выбор: ", "Ошибка, введите 1 или 2")
    if action not in (1,2):
        return

    if action == 1:
        gen = safe_int_input("1. Ввести ключи вручную\n2. Сгенерировать автоматически\nВаш выбор: ", "Ошибка, введите 1 или 2")
        if gen == 1:
            n = int(input("Введите n: "))
            e = int(input("Введите e: "))
            d = int(input("Введите d: "))
        else:
            n, e, d = rsa_generate_keys()
            print(f"Сгенерированные ключи:\n n = {n}\n e = {e}\n d = {d}")
            print("Сохраните эти ключи для расшифровки!")
    else:
        n = int(input("Введите n: "))
        d = int(input("Введите d: "))

    outname = "encrypted.bin" if action == 1 else "decrypted.bin"
    with open(outname, "wb") as fout:
        if action == 1:
            for b in data:
                enc = mod_pow(b, e, n)
                fout.write(enc.to_bytes((enc.bit_length()+7)//8 or 1, 'big'))
            print(f"Данные зашифрованы в файл '{outname}'.")
        else:
            with open(fname, "rb") as fin:
                while True:
                    chunk = fin.read((n.bit_length()+7)//8)
                    if not chunk:
                        break
                    enc = int.from_bytes(chunk, 'big')
                    dec = mod_pow(enc, d, n)
                    fout.write(bytes([dec]))
            print(f"Данные расшифрованы в файл '{outname}'.")

    print()

def task5():
    a, b = 237, 44
    d, x, y = extended_gcd(a, b)
    if d == 1:
        print(f"Уравнение {a}*x + {b}*y = 1 имеет решение:")
        print(f"x = {x}, y = {y}")
        print(f"Проверка: {a}*{x} + {b}*{y} = {a*x + b*y}")
    else:
        print("Уравнение не имеет решений, так как НОД ≠ 1")
    print()

def task6():
    try:
        with open("encrypted.bin", "rb") as f:
            blocks = [int.from_bytes(f.read(8), 'big') for _ in range(10)]
            blocks = [b for b in blocks if b is not None]
    except IOError:
        print("Ошибка чтения файла.")
        return

    if not blocks:
        print("Файл пуст или повреждён.")
    else:
        print("Первые зашифрованные блоки:")
        for i, b in enumerate(blocks):
            print(f"Блок {i}: {b}")
        if all(b == blocks[0] for b in blocks):
            print("Все блоки одинаковые – RSA детерминирован и уязвим без паддинга.")
        else:
            print("Блоки отличаются. Вероятно, входной файл не имел повторяющихся данных.")
    print()

def main():
    while True:
        choice = show_menu()
        if choice == '1': task1()
        elif choice == '2': task2()
        elif choice == '3': task3()
        elif choice == '4': task4()
        elif choice == '5': task5()
        elif choice == '6': task6()
        elif choice == '0':
            print("Проверка завершена.")
            sys.exit(0)
        else:
            print("Неверный выбор, попробуйте снова.\n")

if __name__ == "__main__":
    main()

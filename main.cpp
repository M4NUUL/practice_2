#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>

#include "ferm.h"
#include "evclid.h"
#include "rsa.h"

typedef __uint128_t u128;
typedef __int128_t i128;

enum class ActionType
{
   Encrypt = 1,
   Decrypt = 2
};

std::istream &operator>>(std::istream &is, u128 &value)
{
   std::string s;
   is >> s;

   u128 result = 0;
   for (char c : s)
   {
      if (c < '0' || c > '9')
      {
         is.setstate(std::ios::failbit);
         return is;
      }
      result = result * 10 + (c - '0');
   }
   value = result;
   return is;
}

std::ostream &operator<<(std::ostream &os, u128 value)
{
   if (value == 0)
   {
      os << '0';
      return os;
   }
   std::string result;
   while (value > 0)
   {
      result = char('0' + (value % 10)) + result;
      value /= 10;
   }
   os << result;
   return os;
}

std::istream &operator>>(std::istream &is, i128 &value)
{
   std::string s;
   is >> s;

   if (s.empty())
   {
      is.setstate(std::ios::failbit);
      return is;
   }

   i128 result = 0;
   size_t pos = 0;
   bool negative = false;

   if (s[0] == '-')
   {
      negative = true;
      pos = 1;
      if (s.size() == 1)
      { // только '-' — ошибка
         is.setstate(std::ios::failbit);
         return is;
      }
   }

   for (; pos < s.size(); ++pos)
   {
      char c = s[pos];
      if (c < '0' || c > '9')
      {
         is.setstate(std::ios::failbit);
         return is;
      }
      result = result * 10 + (c - '0');
   }

   value = negative ? -result : result;
   return is;
}

std::ostream &operator<<(std::ostream &os, i128 value)
{
   if (value == 0)
   {
      os << '0';
      return os;
   }

   std::string result;
   bool negative = value < 0;
   if (negative)
      value = -value;

   while (value > 0)
   {
      result = char('0' + (value % 10)) + result;
      value /= 10;
   }

   if (negative)
      result = '-' + result;

   os << result;
   return os;
}


bool safeInput(int &number, const std::string &Error)
{
   if (!(std::cin >> number))
   {
      std::cin.clear();                                                   // Очистка флага ошибки
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
      std::cout << Error << std::endl;
      return false;
   }
   else
   {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
      return true;
   }
}

void showMenu()
{
   std::cout << "Выберете задачу для проверки:" << std::endl;
   std::cout << "1. Проверка задания №1" << std::endl;
   std::cout << "2. Проверка задания №2" << std::endl;
   std::cout << "3. Проверка задания №3" << std::endl;
   std::cout << "4. Проверка задания №4" << std::endl;
   std::cout << "5. Проверка задания №5" << std::endl;
   std::cout << "6. Проверка задания №6" << std::endl;
   std::cout << "7. Проверка задания №7" << std::endl;
   std::cout << "0. Выход" << std::endl;
   std::cout << "Выбор: ";
}

int main()
{
   try
   {
      showMenu();
      while (true)
      {
         int choice = -1;
         std::cin >> choice;
         if (std::cin.fail())
         {
            std::cin.clear();                                                   // сброс failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка мусора
            throw std::runtime_error("Введено не число");
         }

         switch (choice)
         {
         case 1:
         {
            std::cout << "Введите a, x и p через пробел: ";
            i128 a, exp, p;
            std::cin >> a >> exp >> p;

            if (!isPrime(p))
            {
               std::cout << "Число не простое, теорема Ферма не применима." << std::endl;
               std::cout << std::endl;
               showMenu();
               break;
            }

            i128 result = theoremFerma(a, exp, p);
            std::cout << "Результат бинарным разложением: " << result << std::endl;
            result = fastPowerLog(a, exp, p);
            std::cout << "Результат через быстрое разложение по модулю: " << result << std::endl;
            std::cout << std::endl;
            showMenu();
            break;
         }
         case 2:
         {
            std::cout << "Введите c и m через пробел: ";
            i128 c, m;
            std::cin >> c >> m;
            i128 d = inverseElement(c, m);

            if (d == -1)
            {
               std::cout << "Решение невозможно, НОД(c, m) != 1" << std::endl;
            }

            else
            {
               std::cout << "Решение: d = " << d << std::endl;
               std::cout << "Проверка: " << c << "*" << d << " mod" << m << " = " << (c * d) % m << std::endl;
            }

            std::cout << std::endl;
            showMenu();
            break;
         }
         case 3:
         {
            std::cout << "Введите c и m через пробел: ";
            i128 c, m;
            std::cin >> c >> m;

            i128 inverse_elem = inverseElement(c, m);
            if (inverse_elem == -1)
            {
               std::cout << "Обратного элемента не существует" << std::endl;
            }

            else
            {
               std::cout << "Обратный элемент: " << c << "^-1 mod " << m << " = " << inverse_elem << std::endl;
               std::cout << "Проверка: " << c << " * " << inverse_elem << " mod " << m << " = " << (c * inverse_elem) % m << std::endl;
            }

            std::cout << std::endl;
            showMenu();
            break;
         }
         case 4:
         {
            std::string filename;
            std::cout << "\nВведите имя бинарного файла (например, image.png): ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка ввода
            std::getline(std::cin, filename);

            std::ifstream inFile(filename, std::ios::binary);
            if (!inFile)
            {
               std::cerr << "Ошибка: файл не найден.\n";
               showMenu();
               break;
            }

            std::vector<uint8_t> input_data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            inFile.close();
            std::cout << "Файл успешно прочитан (" << input_data.size() << " байт).\n";

            int action_choice;
            std::cout << "Выберите действие:\n1. Зашифровать\n2. Дешифровать\nВаш выбор: ";
            if (!safeInput(action_choice, "Ошибка, введите 1 или 2"))
            {
               showMenu();
               break;
            }

            if (action_choice != 1 && action_choice != 2)
            {
               std::cerr << "Ошибка: введите число 1 или 2.\n";
               showMenu();
               break;
            }

            uint64_t n = 0, e = 0, d = 0;

            if (action_choice == 1)
            {
               int gen_choice;
               std::cout << "Выберите способ получения ключей:\n1. Ввести вручную\n2. Сгенерировать автоматически\nВаш выбор: ";
               if (!safeInput(gen_choice, "Ошибка, введите 1 или 2"))
               {
                  showMenu();
                  break;
               }

               if (gen_choice == 1)
               {
                  std::cout << "Введите ключи n e d через пробел: ";
                  if (!(std::cin >> n >> e >> d))
                  {
                     std::cerr << "Ошибка: неверный формат ключей.\n";
                     std::cin.clear();
                     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                     showMenu();
                     break;
                  }
                  std::cin.ignore();
               }
               else
               {
                  std::tie(n, e, d) = rsaGenerateKeys();
                  std::cout << "Сгенерированные ключи:\n n = " << n << "\n e = " << e << "\n d = " << d << "\n";
                  std::cout << "Сохраните эти ключи для расшифровки!" << std::endl;
               }
            }
            else
            {
               std::cout << "Введите открытый модуль (n) и закрытую экспоненту (d) через пробел: ";
               if (!(std::cin >> n >> d))
               {
                  std::cerr << "Ошибка: неверный формат ключей.\n";
                  std::cin.clear();
                  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                  showMenu();
                  break;
               }
               std::cin.ignore();
            }

            std::string outname = (action_choice == 1) ? "encrypted.bin" : "decrypted.bin";
            std::ofstream outFile(outname, std::ios::binary);
            if (!outFile)
            {
               std::cerr << "Ошибка при открытии файла для записи.\n";
               showMenu();
               break;
            }

            if (action_choice == 1)
            {
               for (uint8_t byte : input_data)
               {
                  uint64_t encrypted = modPow(byte, e, n);
                  outFile.write(reinterpret_cast<const char *>(&encrypted), sizeof(encrypted));
               }
               std::cout << "Данные зашифрованы в файл '" << outname << "'.\n";
            }
            else
            {
               std::ifstream inEncrypted(filename, std::ios::binary);
               if (!inEncrypted)
               {
                  std::cerr << "Ошибка при открытии файла для расшифровки.\n";
                  showMenu();
                  break;
               }

               while (true)
               {
                  uint64_t encrypted = 0;
                  inEncrypted.read(reinterpret_cast<char *>(&encrypted), sizeof(encrypted));
                  if (inEncrypted.gcount() < sizeof(encrypted))
                     break;

                  uint64_t decrypted = modPow(encrypted, d, n);
                  uint8_t byte = static_cast<uint8_t>(decrypted);
                  outFile.write(reinterpret_cast<const char *>(&byte), 1);
               }

               std::cout << "Данные расшифрованы в файл '" << outname << "'.\n";
            }

            outFile.close();
            std::cout << std::endl;
            showMenu();
            break;
         }

         case 5:
         {
            i128 a = 237;
            i128 b = 44;
            i128 x, y;
            i128 d = extendedGCD(a, b, x, y);
            if (d == 1)
            {
               std::cout << "Уравнение " << a << "*x + " << b << "*y = 1 имеет решение:" << std::endl;
               std::cout << "x = " << x << ", y = " << y << std::endl;

               // Проверка:
               std::cout << "Проверка: " << a << "*" << x << " + " << b << "*" << y << " = " << a * x + b * y << std::endl;
            }
            else
            {
               std::cout << "Уравнение не имеет решений, так как НОД ≠ 1" << std::endl;
            }

            std::cout << std::endl;
            showMenu();
            break;
         }
         case 6:
         {
            std::ifstream attackTest("encrypted.bin", std::ios::binary);
            std::cout << "\n--- Эмуляция атаки: анализ повторяющихся блоков ---\n";

            std::vector<uint64_t> encrypted_blocks;
            for (int i = 0; i < 10 && !attackTest.eof(); ++i)
            {
               uint64_t block = 0;
               attackTest.read(reinterpret_cast<char *>(&block), sizeof(block));
               if (attackTest.gcount() == sizeof(block))
                  encrypted_blocks.push_back(block);
            }

            if (!encrypted_blocks.empty())
            {
               std::cout << "Первые зашифрованные блоки:\n";
               for (size_t i = 0; i < encrypted_blocks.size(); ++i)
                  std::cout << "Блок " << i << ": " << encrypted_blocks[i] << "\n";

               bool all_same = true;
               for (size_t i = 1; i < encrypted_blocks.size(); ++i)
                  if (encrypted_blocks[i] != encrypted_blocks[0])
                     all_same = false;

               if (all_same)
                  std::cout << "Все блоки одинаковые — это показывает, что RSA детерминирован и уязвим без паддинга.\n";
               else
                  std::cout << "Блоки отличаются. Вероятно, входной файл не имел повторяющихся данных.\n";
            }
            else
            {
               std::cout << "Файл пуст или повреждён.\n";
            }

            attackTest.close();

            std::cout << std::endl;
            showMenu();
            break;
         }
         case 7:
         {
            std::cout << std::endl;
            showMenu();
            break;
         }
         case 0:
            std::cout << std::endl;
            std::cout << "Проверка завершена." << std::endl;
            return 0;
         default:
            std::cout << std::endl;
            std::cout << "Неверный выбор, попробуйте снова." << std::endl;
            showMenu();
         }
      }
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << '\n';
   }
   return 0;
}

std::istream &operator>>(std::istream &is, __uint128_t &value)
{
   std::string s;
   is >> s;

   __uint128_t result = 0;
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

std::ostream &operator<<(std::ostream &os, __uint128_t value)
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

std::istream &operator>>(std::istream &is, __int128_t &value)
{
   std::string s;
   is >> s;

   __int128_t result = 0;
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

std::ostream &operator<<(std::ostream &os, __int128_t value)
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
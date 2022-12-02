#include <iostream>

int main() {
  std::string str = "f 2";
  std::string f = "f %d/%d/%d";
  int a = 0, b = 0, c = 0;

  std::cout << sscanf(str.data(), f.data(), &a, &b, &c) << "\n";
  std::cout << a << " a\n";
  std::cout << b << " b\n";
  std::cout << c << " c\n";
}
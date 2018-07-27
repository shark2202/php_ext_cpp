#include <iostream>
 
auto make_function(int& x) {
  return [&]{ std::cout << x << '\n'; };
}
 
int main() {
  int i = 3;
  auto f = make_function(i); // x 于 f 中的使用直接绑定到 i
  i = 5;
  f(); // OK ：打印 5

  return 0;
}
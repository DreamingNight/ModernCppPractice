#include <cstdio>
#include <stdio.h>

class Obj {
public:
  Obj() { puts("Obj()"); }
  ~Obj() { puts("~Obj()"); }
};

void foo(int n) {
  Obj obj;
  if (n == 42) {
    throw "life is so short";
  }
}

int main() {
  try {
    foo(30);
    foo(42);
  } catch (const char *s) {
    puts(s);
  }
  // 对于在栈上的变量，编译器会在合适的位置调用构造和析构函数，不受是否发生异常等影响
}
Чтобы написать код в C++, который будет деактивирован в зависимости от параметра сборки, вы можете использовать директивы препроцессора, такие как `#ifdef`, `#ifndef`, `#if`, `#else` и `#endif`. Эти директивы позволяют включать или исключать код на основе определенных условий, которые могут быть установлены в процессе компиляции.

Вот пример кода на C++, который будет деактивирован, если определен параметр `DEACTIVATE_CODE`:

```cpp
// example.cpp
#include <iostream>

void activeFunction() {
    std::cout << "This function is active." << std::endl;
}

void deactivatedFunction() {
    std::cout << "This function is deactivated." << std::endl;
}

int main() {
    #ifdef DEACTIVATE_CODE
    deactivatedFunction();
    #else
    activeFunction();
    #endif

    return 0;
}
```

Теперь, чтобы управлять деактивацией этого кода с помощью CMake, вы можете использовать определение компилятора. Вот пример `CMakeLists.txt`, который деактивирует код, если параметр `DEACTIVATE_CODE` установлен:

```cmake
cmake_minimum_required(VERSION 3.10)
project(ExampleProject)

# Определить переменную DEACTIVATE_CODE
option(DEACTIVATE_CODE "Deactivate some code" OFF)

# Установить флаг компилятора в зависимости от DEACTIVATE_CODE
if(DEACTIVATE_CODE)
    target_compile_definitions(example PRIVATE DEACTIVATE_CODE)
endif()

# Создать испо��няемый файл
add_executable(example example.cpp)
```

В этом примере мы определили переменную `DEACTIVATE_CODE` с помощью `option()`, которая позволяет пользователю включить или отключить определение компилятора `DEACTIVATE_CODE` при запуске `cmake`. Если пользователь решит включить это определение, код в функции `deactivatedFunction()` будет активирован, в противном случае будет использоваться `activeFunction()`.

Чтобы включить деактивацию кода при сборке, пользователь должен выполнить следующую команду:

```bash
cmake -DDEACTIVATE_CODE=ON ..
```

Если пользователь хочет не деактивировать код, он должен выполнить команду без флага `-DDEACTIVATE_CODE=ON`:

```bash
cmake ..
```

Этот подход позволяет гибко управлять активацией и деактивацией кода в зависимости от параметров сборки..
# Contribution

## Installation requirements

1. Проект использует модули из стандарта C++20, поэтому компилятор и build-система должны поддерживать эту фукнциональность и стандарт:

    1. CMake версии `>=3.28`. В качестве build-генераторы необходимо использовать на выбор:
        1. Ninja `>=1.11`;
        1. Ninja Multi-Config
        1. Visual Studio 2022, toolset версии `>=19.34` (можно попробовать `>=17.4`);
        1. **Unix Makefiles не поддерживают C++20 модули!**
        > See the cmake-cxxmodules(7) manual for details

    1. Компиляторы:
        1. clang `>=19`.

### Установка под Mac через Homebrew:

Будем использовать `Ninja+clang`.

1. Установить `Ninja` (см. [тут](https://formulae.brew.sh/formula/ninja)):
```bash
brew install ninja
```

1. Установка компилятора (см. [тут](https://formulae.brew.sh/formula/llvm)):
```bash
brew install llvm

# For compilers to find llvm you may need to set:
#   export LDFLAGS="-L/usr/local/opt/llvm/lib"
#   export CPPFLAGS="-I/usr/local/opt/llvm/include"
```

Если ранее уже был установлен `clang` через средства XCode, то можно добавить в `~/.bash_profile`:
```bash
PATH="/usr/local/opt/llvm/bin:${PATH}"
export PATH
```
Так установленная версия через `brew` будет в приоритете.



## Структура проекта

1. `src` - компоненты библиотеки.
1. `tests` - тест для компонент библиотеки.
1. `playground` - песочница для тестирования библиотеки в различных сценариях.
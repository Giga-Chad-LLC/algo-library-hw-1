# Building

## Installation requirements

1. Проект использует модули из стандарта C++20, поэтому компилятор и build-система должны поддерживать эту фукнциональность и стандарт:

    1. CMake версии `>=3.28`. В качестве build-генераторы необходимо использовать на выбор:
        1. Ninja `>=1.11` (**Unix Makefiles не поддерживают C++20 модули!** Поэтому проект собирается только Ninja)
        1. Visual Studio 2022, toolset версии `>=19.34` (можно попробовать `>=17.4`)
        > See the cmake-cxxmodules(7) manual for details

    1. Компиляторы:
        1. clang `>=19`.

### Установка под Linux:

В дистрибутивах убунты обычно даже с обновленными пакетными репозиториями не будет clang 19-ой версии и
прочих зависимостей, нужной нам версии. Поэтому мы ставим часть из них через `pip`:


1. Установить `Ninja` (см. [тут](https://formulae.brew.sh/formula/ninja)):
    ```bash
    sudo apt update
    sudo apt install python3-pip

    pip install ninja==1.11
    ninja --version # может понадобиться добавить его в PATH
    ```

1.  Установить clang-19 и сделать его дефолтым компилятором в системе:
    ```bash
    # Скачиваем установочный скрипт llvm 
    wget https://apt.llvm.org/llvm.sh
    chmod +x llvm.sh
    # Запускаем скрипт, добавляющий репозиторий с LLVM/Clang 19    
    sudo ./llvm.sh 19
    
    # Устанавливаем
    sudo apt-get update
    sudo apt-get install -y clang-19 lldb-19 lld-19 clangd-19
    
    # Делаем clang-19 дефолтным компилятором
    sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 100
    sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 100
    
    # Проверяем
    clang --version
    clang++ --version
    ```

### Установка под Mac (через Homebrew):

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

### Установка под Windows:

Удачи)

## Структура проекта

1. `src` - компоненты библиотеки.
1. `tests` - юнит-тесты для компонент библиотеки.
1. `playground` - песочница для тестирования библиотеки в различных сценариях.
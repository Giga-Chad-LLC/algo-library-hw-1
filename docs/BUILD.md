# Building

## Installation requirements

1. Проект использует модули из стандарта C++20, поэтому компилятор и build-система должны поддерживать эту фукнциональность и стандарт:

    1. CMake версии `>=3.28`. В качестве build-генераторы необходимо использовать на выбор:
        1. Ninja `>=1.11` (**Unix Makefiles не поддерживают C++20 модули!** Поэтому проект собирается только Ninja)
        1. Visual Studio 2022, toolset версии `>=19.34` (можно попробовать `>=17.4`)
        > See the cmake-cxxmodules(7) manual for details

    1. Компиляторы:
        1. clang `>=19`
        2. _Кажется_: GCC `>=14`?


### Для всех (если планируете запускать benchmarking):

1. Установить Python (в целом любой версии, какой-нибудь свежей). Смотрим [тут](https://www.python.org/downloads/).

2. Делаем virtual env (что это такое [тут](https://docs.python.org/3/library/venv.html)):
```bash
# создаем venv
python -m venv ./.venv
# активируем в текущем shell
. .venv/bin/activate
# проверяем, что указывает в созданную папку .venv
which python
# устанавливаем зависимости проекта в virtual env (не глобально! что и хотели!)
pip install -r ./requirements.txt
```

Чтобы деактивировать virtual env:
```bash
(.venv) ~/algo-assignment: deactivate
~/algo-assignment:
```

### Установка под Linux:

**Рекомендация: делайте все через Docker и image `ubuntu`**. Например, через `24.04`, можно и `22.04`, и старее:
```bash
# Скачали image
docker pull ubuntu:24.04
# Запускаем container с image'а
docker run -it --name [my-ubuntu-container-name] ubuntu:24.04 bash
```

1. Установить `CMake >=3.28`:

В `ubuntu:24.04` установка CMake через `sudo apt-get -y install cmake` должна давать версию `>=3.28`, для более старых можно так:
```bash
wget https://github.com/Kitware/CMake/releases/download/v3.29.4/cmake-3.29.4-linux-x86_64.sh
chmod +x cmake-3.29.4-linux-x86_64.sh
sudo ./cmake-3.29.4-linux-x86_64.sh --skip-license --prefix=/usr/local
```

1. Установить `Ninja` (см. [тут](https://formulae.brew.sh/formula/ninja)):

В дистрибутивах убунты обычно даже с обновленными пакетными репозиториями не будет clang 19-ой версии и
прочих зависимостей, нужной нам версии. Поэтому мы ставим часть из них через `pip`:

```bash
sudo apt update
sudo apt install python3-pip

pip install ninja==1.11
ninja --version # может понадобиться добавить его в PATH
```

Альтернативно:
```bash
sudo wget -O /usr/local/bin/ninja https://github.com/ninja-build/ninja/releases/latest/download/ninja-linux.zip
sudo chmod a+x /usr/local/bin/ninja
```

Для `ubuntu:24.04` можно через apt:
```bash
sudo apt update
sudo apt install ninja-build
```

**Важно: если случайно запустили CMake не с Ninja, а с другим build-тулом, то папку `build` надо удалить перед сменой build-тула**.

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

Сперва поставьте Homebrew, следуя инструкции [тут](https://brew.sh).

Будем использовать `Ninja+clang`.

1. Установить `CMake >= ` (см. [тут](https://formulae.brew.sh/formula/cmake)):
    ```bash
    brew install cmake
    ```

1. Установить `Ninja >=1.11` (см. [тут](https://formulae.brew.sh/formula/ninja)):
    ```bash
    brew install ninja
    ```

1. Установка компилятора `clang >=19` (см. [тут](https://formulae.brew.sh/formula/llvm)):
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
Так установленная версия через `brew` будет в приоритете. Проверяем версии через:
```bash
clang --version
clang++ --version
```

### Установка под Windows:

Мы не знаем 😳. Терпеть! 

Узнаете - сделайте PR, пожалуйста, с описанием. 

## Сборка и запуск CMake таргетов

1. Сборка:
```bash
# запустит сначала `make configure`, а потом `make build`
make setup
# аналог:
make configure
make build
```

В проекте два запускаемых таргета:
1. Unit-тесты:
```bash
make test
```

2. Benchmarking:
```bash
make playground
```
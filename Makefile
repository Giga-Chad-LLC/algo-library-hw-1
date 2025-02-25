# Installation for CI
.PHONY: install
install:
	sudo apt-get -y update
	sudo apt-get install -y wget lsb-release software-properties-common
	make install-llvm
	make install-cmake
	make install-ninja
	make install-valgrind
	make install-python
	make install-python-requirements


# Clang and LLVM
.PHONY: install-llvm
install-llvm:
	wget https://apt.llvm.org/llvm.sh
	chmod +x llvm.sh
	sudo ./llvm.sh 19
	sudo apt-get install clang-tools-19
	sudo update-alternatives --remove-all clang || true
	sudo update-alternatives --remove-all clang++ || true
	sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 100
	sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 100
	sudo update-alternatives --set clang /usr/bin/clang-19
	sudo update-alternatives --set clang++ /usr/bin/clang++-19
	clang++ --version

# CMake
.PHONY: install-cmake
install-cmake:
	sudo apt-get -y install cmake
	cmake --version

# Ninja
.PHONY: install-ninja
install-ninja:
	sudo apt-get -y install ninja-build
	ninja --version

# Valgrind
.PHONY: install-valgrind
install-valgrind:
	sudo apt-get -y install valgrind
	valgrind --version

.PHONY: install-python
install-python:
	sudo apt-get -y install python
	python3 --version

.PHONY: install-python-requirements
install-python-requirements:
	pip3 install -r ./requirements.txt


# Build setup
.PHONY: build-ci
build-ci: config-ci build

.PHONY: config-ci
config-ci:
	cmake -S . -B build -G Ninja \
		-DCMAKE_C_COMPILER=/usr/bin/clang \
		-DCMAKE_CXX_COMPILER=/usr/bin/clang++


.PHONY: config-local
config-local:
	cmake -S . -B build -G Ninja \
		-DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang \
		-DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++


.PHONY: build
build:
	cmake --build build


# Executables
.PHONY: test
test:
	./build/tests/test_tree_algorithms


.PHONY: playground
playground:
	./scripts/benchmarking.py -b ./build/playground/playground_tree_algorithms

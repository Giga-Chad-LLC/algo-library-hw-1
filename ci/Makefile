###################################################################
#### This Makefile is intended to be used ONLY by CI pipeline! ####
#### For local development and setup refer to and modify the   ####
#### `Makefile`located at the project root. 				   ####
###################################################################


# Build setup
.PHONY: ci-setup
ci-setup: ci-config ci-build

# CMake initial configuration step
.PHONY: ci-config
ci-config:
	cmake -S . -B build -G Ninja \
		-DCMAKE_C_COMPILER=/usr/bin/clang \
		-DCMAKE_CXX_COMPILER=/usr/bin/clang++
# CMake build
.PHONY: ci-build
ci-build:
	cmake --build build


# Executing runnable targets
.PHONY: ci-test
ci-test:
	./build/tests/test_tree_algorithms

.PHONY: ci-playground
ci-playground:
	python3 ./scripts/benchmarking.py -b ./build/playground/playground_tree_algorithms -c ./configs/benchmarking.yaml



#### CI Installation Steps ####

# CI full installation: LLVM, CMake+Ninja, Valgrind, Python
.PHONY: ci-install
ci-install:
	sudo apt-get -y update
	sudo apt-get install -y wget lsb-release software-properties-common
	make ci-install-llvm
	make ci-install-cmake
	make ci-install-ninja
	make ci-install-valgrind
	make ci-install-python
	make ci-install-python-requirements


# Clang and LLVM
.PHONY: ci-install-llvm
ci-install-llvm:
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
.PHONY: ci-install-cmake
ci-install-cmake:
	sudo apt-get -y install cmake
	cmake --version

# Ninja
.PHONY: ci-install-ninja
ci-install-ninja:
	sudo apt-get -y install ninja-build
	ninja --version

# Valgrind
.PHONY: ci-install-valgrind
ci-install-valgrind:
	sudo apt-get -y install valgrind
	valgrind --version

.PHONY: ci-install-python
ci-install-python:
	sudo apt-get -y install python3 python3-pip
	python3 --version

.PHONY: ci-install-python-requirements
ci-install-python-requirements:
	pip3 install -r ./requirements.txt
	pip3 list

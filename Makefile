# Use `which` to find the paths of clang and clang++
CC := $(shell which clang)
CXX := $(shell which clang++)

.PHONY: setup
setup: configure build

.PHONY: configure
configure:
	cmake -S . -B build -G Ninja \
		-DCMAKE_C_COMPILER=$(CC) \
		-DCMAKE_CXX_COMPILER=$(CXX)

.PHONY: build
build:
	cmake --build build


# Executables
.PHONY: test
test:
	./build/tests/test_tree_algorithms


.PHONY: playground
playground:
	python ./scripts/benchmarking.py -b ./build/playground/playground_tree_algorithms -c ./configs/benchmarking.yaml

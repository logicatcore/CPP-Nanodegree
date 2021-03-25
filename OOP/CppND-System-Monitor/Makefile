.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: build_test
build_test:
	mkdir -p build
	cd build && \
	cmake .. && \
	make
	./build/testt

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug -std=c++17 .. && \
	make

.PHONY: debug_test
debug_test:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug -std=c++17 .. && \
	make
	./build/testt

.PHONY: clean
clean:
	rm -rf build

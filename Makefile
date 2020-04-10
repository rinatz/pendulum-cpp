CONFIG = Debug
OUT = build/$(CONFIG)
CMAKE_SOURCE_DIR = $(CURDIR)

.PHONY: get
get:
	conan install -u .

.PHONY: cmake
cmake:
	mkdir -p $(OUT)
	cd $(OUT) && cmake -DCMAKE_BUILD_TYPE=$(CONFIG) $(CMAKE_SOURCE_DIR)

.PHONY: build
build: cmake
	cmake --build $(OUT)

.PHONY: test
test: build
	-$(OUT)/pendulum_test

.PHONY: clean
clean:
	cmake --build $(OUT) --target clean

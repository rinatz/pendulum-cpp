PROJECT_DIR = $(CURDIR)
BUILD_TYPE = Debug
OUT_DIR = target/$(BUILD_TYPE)
DEPS_DIR = $(OUT_DIR)/deps
BUILD_DIR = $(OUT_DIR)/build

.PHONY: install
install:
	mkdir -p $(DEPS_DIR)
	cd $(DEPS_DIR) && conan install --build=missing $(PROJECT_DIR)

.PHONY: update
update:
	mkdir -p $(DEPS_DIR)
	cd $(DEPS_DIR) && conan install -u --build=missing $(PROJECT_DIR)

.PHONY: cmake
cmake: install
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(PROJECT_DIR)

.PHONY: build
build: cmake
	cmake --build $(BUILD_DIR)

.PHONY: test
test:
	-$(OUT_DIR)/pendulum_test

.PHONY: package
package:
	cd $(DEPS_DIR) && conan export-pkg $(PROJECT_DIR)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)

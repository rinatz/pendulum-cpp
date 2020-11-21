PROJECT_DIR = $(CURDIR)
VERSION := $(shell cat VERSION)

BUILD_TYPE = Debug
OUT_DIR = target/$(BUILD_TYPE)

.PHONY: init
init:
	mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)

.PHONY: install
install: init
	cd $(OUT_DIR) && conan install --build=missing $(PROJECT_DIR)

.PHONY: update
update:
	cd $(OUT_DIR) && conan install -u --build=missing $(PROJECT_DIR)

.PHONY: build
build:
	cd $(OUT_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(PROJECT_DIR)
	cmake --build $(OUT_DIR)

.PHONY: test
test:
	GTEST_COLOR=1 make -C $(OUT_DIR) test ARGS="--verbose"

.PHONY: package
package:
	cd $(OUT_DIR) && conan export-pkg $(PROJECT_DIR) pendulum/$(VERSION)@_/_

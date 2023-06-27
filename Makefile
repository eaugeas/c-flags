CMAKE_BUILD_TYPE ?= Debug
CMAKE_BUILD_GENERATOR ?= Ninja
CMAKE_C_COMPILER ?= gcc
BUILD_BINARY_DIR=build-$(shell echo $(CMAKE_BUILD_TYPE) | tr A-Z a-z)-$(CMAKE_C_COMPILER)

all: build

build: $(BUILD_BINARY_DIR)
	cmake --build $(BUILD_BINARY_DIR) --target all

$(BUILD_BINARY_DIR):
	cmake -B $(BUILD_BINARY_DIR) -S . -G $(CMAKE_BUILD_GENERATOR) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$(CMAKE_C_COMPILER) -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	ln -s $(BUILD_BINARY_DIR)/compile_commands.json compile_commands.json

test: build
	cmake --build $(BUILD_BINARY_DIR) --target test

check: build
	cmake --build $(BUILD_BINARY_DIR) --target c-verify-check-target

fix: build
	cmake --build $(BUILD_BINARY_DIR) --target c-verify-fix-target

clean:
	@rm -rf $(BUILD_BINARY_DIR)
	@rm -f compile_commands.json

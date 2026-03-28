.PHONY: all configure clean install vars

SHELL    := /bin/bash
PRESET    = default
BUILD_DIR = build

all:
	@if [ ! -d "$(BUILD_DIR)" ]; then $(MAKE) configure; fi
	cmake --build --preset $(PRESET)

configure:
	cmake --preset $(PRESET)

clean:
	rm -f -r $(BUILD_DIR)

install:
	cmake --install $(BUILD_DIR)

vars:
	echo "$(PRESET): $(BUILD_DIR)"

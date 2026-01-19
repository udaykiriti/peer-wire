# Simple Makefile wrapper for CMake build system

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin

.PHONY: all build clean rebuild

all: build

$(BUILD_DIR):
	cmake -S . -B $(BUILD_DIR)

build: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --config Release

clean:
	cmake --build $(BUILD_DIR) --target clean || true
	rm -rf $(BUILD_DIR)

rebuild: clean all

tracker:
	./scripts/run_tracker.sh

daemon:
	./scripts/run_daemon.sh 9000 9999

ui:
	./scripts/tui.sh

help:
	@echo "Available targets:"
	@echo "  all       : Build the project"
	@echo "  clean     : Remove build directory"
	@echo "  rebuild   : Clean and build"
	@echo "  tracker   : Run the tracker"
	@echo "  daemon    : Run the peer daemon (background service)"
	@echo "  ui        : Run the TUI (Client)"


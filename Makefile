# Makefile for cross-platform USB Monitor
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Isrc
DIST_DIR = dist

LINUX_SRC = src/main.cpp src/utils/file_utils.cpp src/utils/logger.cpp src/utils/env_loader.cpp src/platform/linux.cpp
WIN_SRC = src/main.cpp src/utils/file_utils.cpp src/utils/logger.cpp src/utils/env_loader.cpp src/platform/windows.cpp

LINUX_TARGET = $(DIST_DIR)/linux
WIN_TARGET = $(DIST_DIR)/win.exe
all: linux win

prepare:
	@mkdir -p $(DIST_DIR)

linux: prepare
	$(CXX) $(CXXFLAGS) $(LINUX_SRC) -o $(LINUX_TARGET) -ludev

win: prepare
	x86_64-w64-mingw32-g++ -std=c++17 -Wall -Iinclude -Isrc $(WIN_SRC) -o $(WIN_TARGET) -luser32 -lgdi32

test-utils: prepare
	$(CXX) $(CXXFLAGS) src/utils/test_file_utils.cpp -o $(DIST_DIR)/test_file_utils
	./$(DIST_DIR)/test_file_utils

test: prepare
	$(CXX) $(CXXFLAGS) tests/test_runner.cpp src/utils/test_file_utils.cpp -o $(DIST_DIR)/test_runner
	./$(DIST_DIR)/test_runner

test-f: prepare
	@if [ -z "$(file)" ]; then \
		echo "❌ Please provide a file to test: make test-f file=filename.cpp"; \
		exit 1; \
	fi
	$(CXX) $(CXXFLAGS) $(file) -o $(DIST_DIR)/test_single
	./$(DIST_DIR)/test_single

clean:
	rm -rf $(DIST_DIR)

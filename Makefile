# Makefile for cross-platform USB Monitor
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = src/main.cpp src/utils/file_utils.cpp
OUT = usb_monitor

ifeq ($(OS),Windows_NT)
    SRC += src/platform/usb_monitor_windows.cpp
    LIBS = -luser32 -lgdi32
    OUT := $(OUT).exe
else
    SRC += src/platform/usb_monitor_linux.cpp
    LIBS = -ludev
endif

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f usb_monitor usb_monitor.exe

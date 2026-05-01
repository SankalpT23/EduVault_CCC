CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -I include

SRC_DIR = src
BUILD_DIR = build

TARGET = eduvault

SOURCES = $(SRC_DIR)/main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(BUILD_DIR)/$(TARGET).exe

run: $(TARGET)
	@$(BUILD_DIR)/$(TARGET).exe

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist data\students.dat del data\students.dat
	@if exist data\users.dat del data\users.dat

.PHONY: all run clean

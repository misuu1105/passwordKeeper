# Directories
SRC_DIR = src
BUILD_DIR = build/debug
INCLUDE_DIR_SDL2 = /opt/homebrew/Cellar/sdl2/2.30.5/include
INCLUDE_DIR_SDL2_IMAGE = /opt/homebrew/Cellar/sdl2_image/2.8.2_1/include
INCLUDE_DIR_SDL2_TTF = /opt/homebrew/Cellar/sdl2_ttf/2.22.0/include
INCLUDE_DIR_TINYXML2 = /opt/homebrew/Cellar/tinyxml2/10.0.0/include
LIB_DIR_SDL2 = /opt/homebrew/Cellar/sdl2/2.30.5/lib
LIB_DIR_SDL2_IMAGE = /opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib
LIB_DIR_SDL2_TTF = /opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib
LIB_DIR_TINYXML2 = /opt/homebrew/Cellar/tinyxml2/10.0.0/lib

CC = g++
CXXFLAGS = -std=c++17 -Wall -O0 -g \
            -I$(INCLUDE_DIR_SDL2) \
            -I$(INCLUDE_DIR_SDL2_IMAGE) \
            -I$(INCLUDE_DIR_SDL2_TTF) \
            -I$(INCLUDE_DIR_TINYXML2)
LINKER_FLAGS = -L$(LIB_DIR_SDL2) -lSDL2 \
               -L$(LIB_DIR_SDL2_IMAGE) -lSDL2_image \
               -L$(LIB_DIR_SDL2_TTF) -lSDL2_ttf \
               -L$(LIB_DIR_TINYXML2) -ltinyxml2

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BUILD_DIR)/output

# Ensure the build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Default target
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CXXFLAGS) $(OBJ_FILES) -o $@ $(LINKER_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

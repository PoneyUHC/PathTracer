
BUILD_DIR = build
EXEC_NAME = render.exe
OUTPUT_DIR = output
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
CFLAGS = -g -O0 -Wall -Wpedantic -std=c++17

default: render.exe

$(EXEC_NAME):
	echo $(SRC)
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	g++ $(SRC) $(CFLAGS) -I $(SRC_DIR) -o $(BUILD_DIR)/$(EXEC_NAME)

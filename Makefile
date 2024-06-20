
BUILD_DIR = build
EXEC_NAME = render.exe
OUTPUT_DIR = output
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
FLAGS = -g -O0

default: render.exe

$(EXEC_NAME):
	echo $(SRC)
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	g++ $(SRC) $(FLAGS) -o $(BUILD_DIR)/$(EXEC_NAME)

run:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	g++ $(SRC) $(FLAGS) -o $(BUILD_DIR)/$(EXEC_NAME)
	$(BUILD_DIR)/$(EXEC_NAME) $(ARGS)

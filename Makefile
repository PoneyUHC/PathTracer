
BUILD_DIR = build
EXEC_NAME = render.exe
OUTPUT_DIR = output
SRC_DIR = src

all:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	g++ $(SRC_DIR)/* -o $(OUTPUT_DIR)/$(EXEC_NAME)

run:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	g++ $(SRC_DIR)/* -o $(OUTPUT_DIR)/$(EXEC_NAME)
	$(OUTPUT_DIR)/$(EXEC_NAME)

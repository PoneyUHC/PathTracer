
CXX = g++
RM = rm -f
CXXFLAGS = -g -O3 -Wall -Wpedantic -std=c++17 -MMD -MP

EXEC = render.exe
OUTPUT_DIR = output
SRC_DIR = src
BUILD_DIR = build
BUILD_OBJS_DIR = $(BUILD_DIR)/binaries
BUILD_EXEC_DIR = $(BUILD_DIR)/executable

SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJS =  $(patsubst %.cpp, %.o, $(SRCS))

OBJS_LOCATION = $(patsubst %.o, $(BUILD_OBJS_DIR)/%.o, $(OBJS))
DEPS =  $(patsubst %.o, %.d, $(OBJS_LOCATION))

.PHONY: all clean

all: $(EXEC)

clean:
	$(RM) $(OBJS_LOCATION) $(DEPS)

$(EXEC): $(OBJS_LOCATION)
	if [ ! -d $(BUILD_EXEC_DIR) ]; then mkdir $(BUILD_EXEC_DIR); fi
	$(CXX) $(OBJS_LOCATION) $(CXXFLAGS) -I $(SRC_DIR) -o $(BUILD_EXEC_DIR)/$(EXEC)

-include $(DEPS)

$(BUILD_OBJS_DIR)/%.o: %.cpp Makefile
	if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CXX) $(CXXFLAGS) -I $(SRC_DIR) -c $< -o $@


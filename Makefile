
CXX = g++
RM = rm -f
CXXFLAGS = -O3 -Wall -Wpedantic -std=c++20 -MMD -MP -fopenmp

EXEC = render.exe
OUTPUT_DIR = output
SRC_DIR = src
BUILD_DIR = build
BUILD_OBJS_DIR = $(BUILD_DIR)/binaries
BUILD_EXEC_DIR = $(BUILD_DIR)/executable

SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/*.cpp)
OBJS =  $(patsubst %.cpp, %.o, $(SRCS))

OBJS_LOCATION = $(patsubst %.o, $(BUILD_OBJS_DIR)/%.o, $(OBJS))
DEPS =  $(patsubst %.o, %.d, $(OBJS_LOCATION))


.PHONY: all clean

all: $(EXEC)

clean:
	$(RM) $(OBJS_LOCATION) $(DEPS)

NB_TARGETS = $(words $(SRCS))
CURRENT_TARGET = 0

$(EXEC): $(OBJS_LOCATION)
	@if [ ! -d $(BUILD_EXEC_DIR) ]; then mkdir -p $(BUILD_EXEC_DIR); fi
	@printf "\nLinking\n"
	@$(CXX) $(OBJS_LOCATION) $(CXXFLAGS) -I $(SRC_DIR) -o $(BUILD_EXEC_DIR)/$(EXEC)
	@printf "Done.\n"

-include $(DEPS)

$(BUILD_OBJS_DIR)/%.o: %.cpp Makefile
	@$(eval CURRENT_TARGET=$(shell expr $(CURRENT_TARGET) + 1))
	@printf "\rCompiling file $(CURRENT_TARGET) / $(NB_TARGETS) "
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	@$(CXX) $(CXXFLAGS) -I $(SRC_DIR) -c $< -o $@


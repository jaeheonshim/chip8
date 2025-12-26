BUILD_DIR := ./build
SRC_DIRS := ./src
LIB_DIRS := ./lib

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
LIBS := $(shell find $(LIB_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
ALL_SRCS := $(SRCS) $(LIBS)

OBJS := $(ALL_SRCS:%=$(BUILD_DIR)/%.o)

INC_FLAGS := -Ilib -Isrc
CPPFLAGS := $(INC_FLAGS) `fltk-config --cxxflags`
LDFLAGS := `fltk-config --ldflags`

$(BUILD_DIR)/chip8: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
# Project Name (executable)
TARGET_EXE = gcflobdd
TARGET_STATIC_LIB = libgcflobdd.a
# Detect operating system
UNAME_S := $(shell uname -s)

# Set platform-specific library extension
ifeq ($(UNAME_S),Darwin)
    # macOS
    TARGET_DYLIB = libgcflobdd.dylib
else
    TARGET_DYLIB = libgcflobdd.so
endif

# Compiler
CC = g++
SRC_DIR = src
OBJ_DIR = obj

# Run Options       
COMMANDLINE_OPTIONS = #/dev/ttyS0

# Compiler options during compilation
CFLAGS += -g -O3 -std=c++2a -w
CFLAGS += -I$(SRC_DIR)/gcflobdd -I$(SRC_DIR)/utils/
CFLAGS += $(shell pkg-config --cflags libgvc libcgraph)

#Libraries for linking
LIBS = $(shell pkg-config --libs libgvc libcgraph)

#-- Do not edit below this line --

# Subdirs to search for additional source files
SOURCE_FILES += $(wildcard $(SRC_DIR)/gcflobdd/*.cpp)
SOURCE_FILES += $(wildcard $(SRC_DIR)/utils/*.cpp)
SOURCE_FILES += $(wildcard $(SRC_DIR)/ops/*.cpp)
SOURCE_FILES += $(wildcard $(SRC_DIR)/grammar/*.cpp)
SOURCE_FILES += $(wildcard $(SRC_DIR)/visualization/*.cpp)
# SOURCE_FILES += $(shell find . -maxdepth 1 -mindepth 1 -name \*.cpp -a -not -name main.cpp)

# Create an object file of every cpp file
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCE_FILES))

EXE_SOURCE_FILES = $(SOURCE_FILES)
EXE_SOURCE_FILES += $(wildcard $(SRC_DIR)/*.cpp)
EXE_SOURCE_FILES += $(wildcard $(SRC_DIR)/hardware_benchmarks/*.cpp)
# Create an object file of every cpp file
EXE_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(EXE_SOURCE_FILES))

# Make $(PROJECT) the default target
.PHONY: all static

all: $(TARGET_DYLIB) $(TARGET_EXE)

static: $(TARGET_STATIC_LIB)

$(TARGET_EXE): $(EXE_OBJECTS)
	$(CC) -o $(TARGET_EXE) $(EXE_OBJECTS) $(LIBS)

$(TARGET_DYLIB): $(OBJECTS)
	$(CC) -fPIC -shared -o $(TARGET_DYLIB) $(OBJECTS) $(LIBS)

$(TARGET_STATIC_LIB): $(OBJECTS)
	$(AR) rcs $@ $^

# Compile every cpp file to an object
# %.cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

# Build & Run Project
run: $(TARGET_EXE)
	./$(TARGET_EXE) $(COMMANDLINE_OPTIONS)

# Clean & Debug
.PHONY: makefile-debug
makefile-debug:

.PHONY: clean
clean:
	rm -f $(TARGET_EXE) $(TARGET_DYLIB) $(TARGET_STATIC_LIB) $(OBJ_DIR)/*/*.o $(OBJ_DIR)/*.o

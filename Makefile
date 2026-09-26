CC = clang
CXX = clang++
GLSL = glslc

CONFIG ?= Debug
ASAN   ?= 0

CFLAGS = -std=c99 -m64
CXXFLAGS = -std=c++17 -m64 -Wno-nullability-completeness

ifeq ($(CONFIG),Debug)
	CFLAGS += -g -DDEBUG
	CXXFLAGS += -g -DDEBUG
else
	CFLAGS += -O3
	CXXFLAGS += -O3
endif

ifeq ($(ASAN),1)
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
	CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
	LDFLAGS += -fsanitize=address
endif

INCLUDES = -ISource/Runtime -ISource/ThirdParty -ISource/Testbed
LIBS = -lvulkan -lglfw

BUILD = Build
BIN = $(BUILD)/Binaries/$(CONFIG)
OBJ = $(BUILD)/Intermediates

RUNTIME_SRC = $(shell find Source/Runtime -name '*.c')
TESTBED_SRC = $(shell find Source/Testbed -name '*.c')
VMA_SRC = Source/ThirdParty/VMA/vma.cpp

VMA_OBJ = $(OBJ)/Source/ThirdParty/VMA/vma.o
RUNTIME_OBJ = $(RUNTIME_SRC:%.c=$(OBJ)/%.o)
TESTBED_OBJ = $(TESTBED_SRC:%.c=$(OBJ)/%.o)

all: $(BIN)/Testbed Shaders

$(VMA_OBJ): $(VMA_SRC)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Runtime static library
$(BIN)/libRuntime.a: $(RUNTIME_OBJ) $(VMA_OBJ)
	@mkdir -p $(@D)
	ar rcs $@ $^

# Testbed executable
$(BIN)/Testbed: $(TESTBED_OBJ) $(BIN)/libRuntime.a
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile everything recursively
$(OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Commands
clean:
	@rm -rf $(BUILD)
	@rm -rf compile_commands.json

debug:
	$(MAKE) CONFIG=Debug

release:
	$(MAKE) CONFIG=Release

asan:
	$(MAKE) CONFIG=Debug ASAN=1

Shaders:
	@mkdir -p Build/Shaders
	glslc -fshader-stage=frag Source/Shaders/shader_frag.glsl -o Build/Shaders/shader_frag.spv
	glslc -fshader-stage=vert Source/Shaders/shader_vert.glsl -o Build/Shaders/shader_vert.spv

rebuild: clean all

run: all
	@cd Build/Binaries/Debug && ./Testbed

.PHONY: all clean debug release asan rebuild shaders

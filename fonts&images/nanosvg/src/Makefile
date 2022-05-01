
TARGET = application

# debug build?
DEBUG = 0
# optimization
OPT = -Og
# debug output periph regs
OPT += -g3

# Build paths
BUILD_DIR = build
BUILD_ASM_DIR = $(BUILD_DIR)/asm
BUILD_APP_DIR = $(BUILD_DIR)/app
BUILD_CC_DIR  = $(BUILD_DIR)/ccc
BUILD_CPP_DIR = $(BUILD_DIR)/cpp

# recursive find sources files
# sources
ROOT_DIR = .
C_SOURCES   += $(shell find ${ROOT_DIR} -name '*.c')
CPP_SOURCES += $(shell find ${ROOT_DIR} -name '*.cpp')
ASM_SOURCES += $(shell find ${ROOT_DIR} -name '*.s')

# binaries
PREFIX =
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS +=
C_DEFS += '-D GLEW_STATIC'
# C_DEFS += '-D __align(value)=__attribute__((aligned(value)))'
# C_DEFS += '-D __forceinline=__attribute__((always_inline))'
# C_DEFS += '-D __weak=__attribute__((weak))'

# AS includes
AS_INCLUDES = 

# includes
# Find header directories
INCDIR = .
C_INC    =$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
CPP_INC +=$(shell find -L $(INCDIR) -name '*.hpp' -exec dirname {} \; | uniq)
C_INCLUDES  =$(C_INC:%=-I%)
CPP_INCLUDES=$(CPP_INC:%=-I%)

GCCFLAGS += -Wall
GCCFLAGS += -fdata-sections
GCCFLAGS += -ffunction-sections

CFLAGS_STD = -c -Os -w -std=gnu17 $(GCCFLAGS)
CXXFLAGS_STD = -c -Os -w -std=gnu++17 $(GCCFLAGS)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CFLAGS_STD) 
CPPFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CXXFLAGS_STD) 

# # Generate dependency information
# CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# libraries
LIBDIR =
LIBS =  -lc -lm
#LIBS += -lglut /usr/lib/x86_64-linux-gnu/libGL.so
#LIBS += -lCORE -lGLEW
LIBS += -lglfw -lglut -lGL -lGLU -lX11
LIBS += -lGL -lXi -lXext -lX11

LDFLAGS +=  $(LIBDIR) $(LIBS)
LDFLAGS += -lgcc
LDFLAGS += -lstdc++
# default action: build all
all: $(BUILD_APP_DIR)/$(TARGET).elf $(BUILD_APP_DIR)/$(TARGET).hex $(BUILD_APP_DIR)/$(TARGET).bin

## shell color ##
green=\033[0;32m
YELLOW=\033[1;33m
NC=\033[0m
##-------------##

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_CC_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_CPP_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_ASM_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_CC_DIR)/%.o: %.c Makefile | $(BUILD_CC_DIR)
	@echo "\n ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CC_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_CPP_DIR)/%.o: %.cpp Makefile | $(BUILD_CPP_DIR)
	@echo "\n ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CXX) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CPP_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_ASM_DIR)/%.o: %.s Makefile | $(BUILD_ASM_DIR)
	@echo "\n ${green} [compile:] ${YELLOW} $< ${NC}"
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_APP_DIR)/$(TARGET).elf: $(OBJECTS) Makefile | $(BUILD_APP_DIR)
	@echo "\n ${green} [linking:] ${YELLOW} $@ ${NC}"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@ --format=Berkeley
	cp $(BUILD_APP_DIR)/$(TARGET).elf ./
	./$(BUILD_APP_DIR)/$(TARGET).elf
#	$(SZ) $@ --format=SysV --radix=16

$(BUILD_APP_DIR)/%.hex: $(BUILD_APP_DIR)/%.elf | $(BUILD_APP_DIR)
	$(HEX) $< $@
	
$(BUILD_APP_DIR)/%.bin: $(BUILD_APP_DIR)/%.elf | $(BUILD_APP_DIR)
	$(BIN) $< $@	
	
$(BUILD_CC_DIR):
	mkdir -p $@
$(BUILD_CPP_DIR):
	mkdir -p $@
$(BUILD_APP_DIR):
	mkdir -p $@
$(BUILD_ASM_DIR):
	mkdir -p $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
	rm -f application.elf

-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***


TARGET = SDIO

# debug build?
DEBUG = 1
# optimization
OPT = -O2
# debug output periph regs
OPT += -g3

# Build path
BUILD_DIR = build

# recursive find sources files
# sources
ROOT_DIR = .
C_SOURCES   += $(shell find ${ROOT_DIR} -name '*.c')
CPP_SOURCES += $(shell find ${ROOT_DIR} -name '*.cpp')
ASM_SOURCES += $(shell find ${ROOT_DIR} -name 'startup_*.s')

# binaries
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
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
 
#######################################
# CFLAGS
#######################################
CPU = -mcpu=cortex-m7
FPU = -mfpu=fpv5-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS += -DUSE_HAL_DRIVER
C_DEFS += -DUSE_FULL_LL_DRIVER
# C_DEFS += -DARM_MATH_CM7
# C_DEFS += -DHAL_SD_MODULE_ENABLED

C_DEFS += -DSTM32F746xx

# AS includes
AS_INCLUDES = 

# includes
# Find header directories
INCDIR = .
C_INC    =$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
CPP_INC +=$(shell find -L $(INCDIR) -name '*.hpp' -exec dirname {} \; | uniq)
C_INCLUDES  =$(C_INC:%=-I%)
CPP_INCLUDES=$(CPP_INC:%=-I%)

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

GCCFLAGS += -Wall
GCCFLAGS += -fdata-sections
GCCFLAGS += -ffunction-sections
GCCFLAGS += -nostdlib
GCCFLAGS += -fno-threadsafe-statics
GCCFLAGS += --param max-inline-insns-single=500
GCCFLAGS += -fno-rtti
GCCFLAGS += -fno-exceptions
GCCFLAGS += -fno-use-cxa-atexit

CFLAGS_STD = -c -Os -w -std=gnu11 $(GCCFLAGS)
CXXFLAGS_STD = -c -Os -w -std=gnu++14 $(GCCFLAGS)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CFLAGS_STD) 
CPPFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CXXFLAGS_STD) 

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT += $(shell find ${ROOT_DIR} -name '*_FLASH.ld')

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 

FLASH_SIZE=1048576
RAM_SIZE=327680

LDFLAGS = $(MCU)
LDFLAGS += --specs=nano.specs
LDFLAGS += -Wl,--defsym=LD_FLASH_OFFSET=0
LDFLAGS += -Wl,--defsym=LD_MAX_SIZE=$(FLASH_SIZE)
LDFLAGS += -Wl,--defsym=LD_MAX_DATA_SIZE=$(RAM_SIZE)
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref 
LDFLAGS += -Wl,--check-sections
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--entry=Reset_Handler
LDFLAGS += -Wl,--unresolved-symbols=report-all
LDFLAGS += -Wl,--warn-common
LDFLAGS += -Wl,--default-script=$(LDSCRIPT)
LDFLAGS += $(LIBDIR)
LDFLAGS += -L $(LIBS)
LDFLAGS += -Wl,--start-group
LDFLAGS += -Wl,--end-group
LDFLAGS += -lgcc
LDFLAGS += -lstdc++

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CXX) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash
#######################################
flash:
	st-flash --reset write $(BUILD_DIR)/$(TARGET).bin 0x8000000

stm32flash:
	stm32flash -w $(BUILD_DIR)/$(TARGET).bin -v -g 0x0 /dev/ttyUSB0

#######################################
# debug
#######################################
ifeq (1,$(ENABLE_SEMIHOSTING))
debug:
	killall -q openocd ; openocd \
	-f interface/stlink-v2-1.cfg \
	-f target/stm32f7x.cfg \
	-c "init" \
	-c "arm semihosting enable" \
	sleep 1 &
	gdb-multiarch \
	--eval-command "tar ext :3333" \
	$(BUILD_DIR)/$(TARGET).elf \
	--eval-command "load"
else
debug:
	arm-none-eabi-gdb \
	--eval-command "load" \
	-iex ' tar ext | openocd \
	-f interface/stlink-v2-1.cfg \
	-f target/stm32f7x.cfg \
	-c "adapter_khz 1800" \
	-c "interface hla" \
	-c "gdb_port pipe" ' \
	-iex "monitor halt" \
	$(BUILD_DIR)/$(TARGET).elf
endif

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***

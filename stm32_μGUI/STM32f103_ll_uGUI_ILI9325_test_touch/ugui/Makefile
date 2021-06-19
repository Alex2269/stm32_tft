
TARGET = ugui

# debug build?
DEBUG = 1
# optimization
OPT = -O2

# Build path
BUILD_DIR = build

# sources
ROOT_DIR = .
C_SOURCES += $(shell find ${ROOT_DIR} -name '*.c')
CPP_SOURCES += $(shell find ${ROOT_DIR} -name '*.cpp')

# ASM sources
ASM_SOURCES =  \
startup_stm32f103xb.s

#######################################
# binaries
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable
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
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS +=
C_DEFS += -DSTM32F103xB
C_DEFS += -DUSE_FULL_LL_DRIVER
C_DEFS += -DHSE_VALUE=8000000
C_DEFS += -DHSE_STARTUP_TIMEOUT=100
C_DEFS += -DLSE_STARTUP_TIMEOUT=5000
C_DEFS += -DLSE_VALUE=32768
C_DEFS += -DHSI_VALUE=8000000
C_DEFS += -DLSI_VALUE=40000
C_DEFS += -DVDD_VALUE=3300
C_DEFS += -DPREFETCH_ENABLE=1 

# AS includes
AS_INCLUDES = 

# # Find header directories
ROOT_DIR = .
C_INC=$(shell find -L $(ROOT_DIR) -name '*.h' -exec dirname {} \; | uniq)
CPP_INC=$(shell find -L $(ROOT_DIR) -name '*.[hpp]' -exec dirname {} \; | uniq)
C_INCLUDES=$(C_INC:%=-I%)
CPP_INCLUDES=$(CPP_INC:%=-I%)

# C_INCLUDES +=
# C_INCLUDES += -ICore/Inc
# C_INCLUDES += -ICore/hardware
# C_INCLUDES += -ICore/time
# C_INCLUDES += -IDrivers/STM32F1xx_HAL_Driver/Inc
# C_INCLUDES += -IDrivers/CMSIS/Device/ST/STM32F1xx/Include
# C_INCLUDES += -IDrivers/CMSIS/Include

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CPPFLAGS = $(MCU) $(C_DEFS)  $(CPP_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103C8Tx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU)
LDFLAGS += -specs=nosys.specs
LDFLAGS += -specs=nano.specs
LDFLAGS += -T$(LDSCRIPT) $(LIBDIR) $(LIBS)
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref
LDFLAGS += -Wl,--print-memory-usage,--gc-sections

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
# Make flash
#	"write through st-link v2"
# Make stm32flash:
#	"write through uart /dev/ttyUSB0"
#######################################
flash:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

stm32flash:
	stm32flash -w $(BUILD_DIR)/$(TARGET).bin -v -g 0x0 /dev/ttyUSB0
#######################################

#######################################
debug:
	openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg &
	gdb-multiarch $(BUILD_DIR)/$(TARGET).elf
#######################################

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***

### run make SILENT=0 , with  all print.
### SILENT=1 , default silent mode

SILENT ?= 1
ifeq (1,$(SILENT))
.SILENT:
endif

TARGET = application

# debug build?
DEBUG = 1
# optimization
OPT = -Og
# debug output periph regs
OPT += -g3

ENABLE_SEMIHOSTING ?= 0

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
PREFIX = arm-none-eabi-
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
CPU = -mcpu=cortex-m3
FPU =
FLOAT-ABI = -mfloat-abi=soft
MCU = $(CPU) $(FPU) $(FLOAT-ABI) -mthumb

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
ifeq (1,$(ENABLE_SEMIHOSTING))
C_DEFS += -D SEMIHOSTING_ENABLE=1
endif

# C defines
C_DEFS +=
C_DEFS += '-D __align(value)=__attribute__((aligned(value)))'
# C_DEFS += '-D __forceinline=__attribute__((always_inline))'
# C_DEFS += '-D __irq=__attribute__((interrupt("IRQ")))'
# C_DEFS += '-D __packed=__attribute__((__packed__))'
# C_DEFS += '-D __weak=__attribute__((weak))'

# C_DEFS += -D USE_HAL_DRIVER
# C_DEFS += -D USE_FULL_LL_DRIVER
# C_DEFS += -D USE_STDPERIPH_DRIVER
# C_DEFS += -D CALL_ARM_SYSTEM_INIT
# C_DEFS += -D ARM_MATH_CM3
##
# C_DEFS += -D USE_HAL_DRIVER
C_DEFS += -D USE_FULL_LL_DRIVER
##
C_DEFS += -D STM32F103xB

# AS includes
AS_INCLUDES = 

# Includes
# Find header files: h, hh, hpp.
INCDIR      = .
C_INC       = $(shell find -L $(INCDIR) -name '*.h*' -exec dirname {} \; | uniq)
C_INCLUDES  = $(C_INC:%=-I %)

# compile gcc flags
# ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

WARNINGS := -Wall
WARNINGS += -Wextra
WARNINGS += -Wshadow
WARNINGS += -Wundef
WARNINGS += -Wmaybe-uninitialized
WARNINGS += -Wno-unused-function
WARNINGS += -Wno-error=strict-prototypes
WARNINGS += -Wno-error=cpp
WARNINGS += -Wno-unused-parameter
WARNINGS += -Wno-missing-field-initializers
WARNINGS += -Wno-format-nonliteral
WARNINGS += -Wno-cast-qual
WARNINGS += -Wno-switch-default
WARNINGS += -Wno-ignored-qualifiers
WARNINGS += -Wno-error=pedantic
WARNINGS += -Wno-sign-compare
WARNINGS += -Wno-error=missing-prototypes
# WARNINGS += -Wpointer-arith -fno-strict-aliasing
# WARNINGS += -Wuninitialized
# WARNINGS += -Wunreachable-code
# WARNINGS += -Wreturn-type
# WARNINGS += -Wmultichar
# WARNINGS += -Wformat-security
# WARNINGS += -Wdouble-promotion
# WARNINGS += -Wclobbered
# WARNINGS += -Wdeprecated
# WARNINGS += -Wempty-body
# WARNINGS += -Wshift-negative-value
# WARNINGS += -Wtype-limits
# WARNINGS += -Wsizeof-pointer-memaccess
# WARNINGS += -Wpointer-arith

GCCFLAGS += $(WARNINGS)

GCCFLAGS += -fdata-sections
GCCFLAGS += -ffunction-sections
GCCFLAGS += -nostdlib
GCCFLAGS += -fno-threadsafe-statics
GCCFLAGS += --param max-inline-insns-single=500
GCCFLAGS += -fno-rtti
GCCFLAGS += -fno-exceptions
GCCFLAGS += -fno-use-cxa-atexit

CFLAGS_STD = -c -Os -w -std=gnu17 $(GCCFLAGS)
CXXFLAGS_STD = -c -Os -w -std=gnu++17 $(GCCFLAGS)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CFLAGS_STD) 
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wno-discarded-qualifiers
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-implicit-int
CFLAGS += -Wno-incompatible-pointer-types
CPPFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CXXFLAGS_STD) 

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT += $(shell find ${ROOT_DIR} -name '*_FLASH.ld')

FLASH_SIZE=65536
RAM_SIZE=20480

# libraries
LIBDIR =
LIBS = -lc -lm

ifeq (1,$(ENABLE_SEMIHOSTING))
LIBS += -lrdimon
# LIBS += --specs=nano.specs -lrdimon_nano # no float printf
else
LIBS += -lnosys
endif

LDFLAGS += -Wl,--defsym=LD_FLASH_OFFSET=0
LDFLAGS += -Wl,--defsym=LD_MAX_SIZE=$(FLASH_SIZE)
LDFLAGS += -Wl,--defsym=LD_MAX_DATA_SIZE=$(RAM_SIZE)
LDFLAGS += $(MCU) -T$(LDSCRIPT)
LDFLAGS += -Wl,-gc-sections,--print-memory-usage,-Map=$(BUILD_CC_DIR)/$(TARGET).map
LDFLAGS += -Wl,--check-sections
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--entry=Reset_Handler
LDFLAGS += -Wl,--unresolved-symbols=report-all
LDFLAGS += -Wl,--warn-common
LDFLAGS += -Wl,--default-script=$(LDSCRIPT)
LDFLAGS += -lgcc
LDFLAGS += -lstdc++
LDFLAGS += -Wl,--start-group
LDFLAGS +=  $(LIBDIR) $(LIBS)
LDFLAGS += -Wl,--end-group

ifeq (1,$(ENABLE_SEMIHOSTING))
LDFLAGS += --specs=rdimon.specs
else
LDFLAGS += --specs=nano.specs
# LDFLAGS += -u _printf_float # printf from newlib-nano
# LDFLAGS += -u _scanf_float # scanf from newlib-nano
endif

# default action: build all
all: $(BUILD_APP_DIR)/$(TARGET).elf $(BUILD_APP_DIR)/$(TARGET).hex $(BUILD_APP_DIR)/$(TARGET).bin

## shell color ##
green=\033[0;32m
YELLOW=\033[1;33m
NC=\033[0m
##-------------##
###
# build the application
###
# list of objects
OBJECTS = $(addprefix $(BUILD_CC_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_CPP_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_ASM_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_CC_DIR)/%.o: %.c Makefile | $(BUILD_CC_DIR)
	@echo " ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CC_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_CPP_DIR)/%.o: %.cpp Makefile | $(BUILD_CPP_DIR)
	@echo " ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CXX) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CPP_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_ASM_DIR)/%.o: %.s Makefile | $(BUILD_ASM_DIR)
	@echo " ${green} [compile:] ${YELLOW} $< ${NC}"
	$(AS) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CC_DIR)/$(notdir $(<:.s=.lst)) $< -o $@

$(BUILD_APP_DIR)/$(TARGET).elf: $(OBJECTS) Makefile | $(BUILD_APP_DIR)
	@echo " ${green} [linking:] ${YELLOW} $@ ${NC} \n"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@ --format=Berkeley
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

###
# clean up
###
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash
#######################################
flash:
	st-flash --reset write $(BUILD_APP_DIR)/$(TARGET).bin 0x8000000

stm32flash:
	stm32flash -w $(BUILD_APP_DIR)/$(TARGET).bin -v -g 0x0 /dev/ttyUSB0

#######################################
# debug
#######################################

#######################################
# Generate OpenOCD config file
OPENOCD_SCRIPT    = 'sc_openocd.cfg'
GDB_PORT          = '3333'
OPENOCD_INTERFACE = 'interface/stlink-v2.cfg'
OPENOCD_TARGET    = 'target/stm32f1x.cfg'
ADAPTER_SPEED     = 'adapter speed 1800'
#######################################

#######################################
ifeq (1,$(ENABLE_SEMIHOSTING))
debug:
	$(shell  echo 'gdb_port $(GDB_PORT)' > $(OPENOCD_SCRIPT))
	$(shell  echo 'source [find $(OPENOCD_INTERFACE)]' >> $(OPENOCD_SCRIPT))
	$(shell  echo 'source [find $(OPENOCD_TARGET)]' >> $(OPENOCD_SCRIPT))
	$(shell  echo 'init' >> $(OPENOCD_SCRIPT))
	$(shell  echo 'arm semihosting enable' >> $(OPENOCD_SCRIPT))
	$(shell  echo $(ADAPTER_SPEED) >> $(OPENOCD_SCRIPT))
	sh tmux.run $(BUILD_APP_DIR)/$(TARGET).elf
else
debug:
	gdb-multiarch \
	--eval-command "load" \
	-iex ' tar ext | openocd \
	-f $(OPENOCD_INTERFACE) \
	-f $(OPENOCD_TARGET) \
	-c "adapter_khz 1800" \
	-c "interface hla" \
	-c "gdb_port pipe" ' \
	-iex "monitor halt" \
	$(BUILD_APP_DIR)/$(TARGET).elf
endif

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***


TARGET = f407vet_app_fsmc

DEBUG = 1

# optimization
OPT = -Og
# debug output periph regs
OPT += -g3

# Build path
BUILD_DIR = build


# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/fsmc.c \
Src/rtc.c \
Src/syscalls.c \
Src/App/osc.c \
Src/App/menu.c \
Src/App/time.c \
Src/Lcd/ili9341.c \
Src/Lcd/lcd_io_fsmc16.c \
Src/Lcd/stm32_adafruit_lcd.c \
Src/Lcd/stm32_adafruit_ts.c \
Src/Ts/soft_spi_ts.c \
Src/Ts/xpt2046.c \
Src/stm32f4xx_it.c \
Src/stm32f4xx_hal_msp.c \
Src/stm32f4xx_hal_timebase_tim.c \
Src/system_stm32f4xx.c \
Src/adc.c \
Src/freertos.c \
Src/dma.c

# source ...
ROOT_DIR  = .
FONT_DIR  = Src/Lcd/Fonts
HAL_DIR   = Drivers/STM32F4xx_HAL_Driver/Src
RTOS_DIR  = Middlewares/Third_Party/FreeRTOS/Source
C_SOURCES   += $(shell find ${HAL_DIR}  -name '*.c')
C_SOURCES   += $(shell find ${FONT_DIR} -name '*.c')
C_SOURCES   += $(shell find ${RTOS_DIR} -name '*.c')
CPP_SOURCES += $(shell find ${ROOT_DIR} -name '*.cpp')
ASM_SOURCES += $(shell find ${ROOT_DIR} -name '*.s')


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
 

CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)


# macros for gcc
# AS defines
AS_DEFS = 
C_DEFS += -DUSE_HAL_DRIVER
C_DEFS += -DSTM32F407xx

# AS includes
AS_INCLUDES = 

# Find header directories
ROOT_DIR = .
C_INC=$(shell find -L $(ROOT_DIR) -name '*.h' -exec dirname {} \; | uniq)
C_INCLUDES=$(C_INC:%=-I%)
CPP_INC=$(shell find -L $(ROOT_DIR) -name '*.hpp' -exec dirname {} \; | uniq)
CPP_INCLUDES=$(CPP_INC:%=-I%)

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS   = $(MCU) $(C_DEFS) $(C_INCLUDES)   $(OPT) -Wall -fdata-sections -ffunction-sections
CPPFLAGS = $(MCU) $(C_DEFS) $(CPP_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
CPPFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS   += -MMD -MP -MF"$(@:%.o=%.d)"
CPPFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = $(shell find ${ROOT_DIR} -name '*_FLASH.ld')

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

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
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000

stm32flash:
	stm32flash -w $(BUILD_DIR)/$(TARGET).bin -v -g 0x0 /dev/ttyUSB0
#######################################
debug:
	arm-none-eabi-gdb \
	-iex 'target extended | openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg \
	-c "adapter_khz 1800" -c "interface hla" -c "gdb_port pipe" ' \
	-iex 'monitor halt' \
	-iex 'monitor tpiu config internal itm_swo.fifo uart off 168000000' \
	-iex 'monitor itm port 0 on' \
	$(BUILD_DIR)/$(TARGET).elf

# 	killall -q openocd ; openocd \
# 	-f interface/stlink-v2.cfg \
# 	-f target/stm32f4x.cfg \
# 	-c "init" \
# 	-c "arm semihosting enable" \
# 	sleep 1 &
# 	arm-none-eabi-gdb \
# 	--eval-command "tar ext :3333" \
# 	$(BUILD_DIR)/$(TARGET).elf \
# 	--eval-command "load"

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***

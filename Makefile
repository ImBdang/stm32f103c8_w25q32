###################################
#           PROJECT NAME          
###################################
PROJECT := main

MCU         := cortex-m3
LD_SCRIPT   := stm32_flash.ld
STARTUP_FILE:= startup_stm32f10x_md.s

###################################
#           TOOLCHAINS            
###################################
TOOLCHAINS_PATH     := /home/bdang/toolchains/gcc-arm-none-eabi-14.3
PREFIX_TOOLCHAINS   := arm-none-eabi
GCC                 := $(PREFIX_TOOLCHAINS)-gcc
CP                  := $(PREFIX_TOOLCHAINS)-objcopy
ASM                 := $(PREFIX_TOOLCHAINS)-as

####################################
#           DIRECTORIES
####################################
USER_INC_DIR        := inc 
USER_SRC_DIR        := src

CMSIS_INC_DIR       := CMSIS/inc
CMSIS_SRC_DIR       := CMSIS/src

STL_INC_DIR         := STL/inc
STL_SRC_DIR         := STL/src

BUILD_DIR           := build
USER_BUILD_DIR      := build/user
CMSIS_BUILD_DIR     := build/CMSIS
STL_BUILD_DIR       := build/STL

STARTUP_DIR         := startup
LINKER_DIR          := linker

####################################
#      SOURCES & OBJECTS LIST
####################################
USER_SRC_FILES      := $(wildcard $(USER_SRC_DIR)/*.c)
CMSIS_SRC_FILES     := $(wildcard $(CMSIS_SRC_DIR)/*.c)
STL_SRC_FILES       := $(wildcard $(STL_SRC_DIR)/*.c)

USER_OBJECTS        := $(USER_SRC_FILES:$(USER_SRC_DIR)/%.c=$(USER_BUILD_DIR)/%.o)
CMSIS_OBJECTS       := $(CMSIS_SRC_FILES:$(CMSIS_SRC_DIR)/%.c=$(CMSIS_BUILD_DIR)/%.o)
STL_OBJECTS         := $(STL_SRC_FILES:$(STL_SRC_DIR)/%.c=$(STL_BUILD_DIR)/%.o)
STARTUP_OBJECT      := $(BUILD_DIR)/startup.o

####################################
#           OPT FLAGS
####################################
CFLAGS  := -mcpu=$(MCU) \
           -mthumb -Wall -O0 -g \
           -ffreestanding \
           -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER \
           -I$(USER_INC_DIR) -I$(CMSIS_INC_DIR) -I$(STL_INC_DIR)

LDFLAGS := -T$(LINKER_DIR)/$(LD_SCRIPT) \
           -Wl,--gc-sections \
           -specs=nano.specs \
           -specs=nosys.specs

####################################
#           MAKE RULES
####################################

.PHONY: all clean flash build_libs

all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex

bin: $(BUILD_DIR)/$(PROJECT).bin
hex: $(BUILD_DIR)/$(PROJECT).hex

# Compile user source files
$(USER_BUILD_DIR)/%.o: $(USER_SRC_DIR)/%.c
	@mkdir -p $(USER_BUILD_DIR)
	$(TOOLCHAINS_PATH)/bin/$(GCC) $(CFLAGS) -c $< -o $@

# Compile CMSIS source files
$(CMSIS_BUILD_DIR)/%.o: $(CMSIS_SRC_DIR)/%.c
	@mkdir -p $(CMSIS_BUILD_DIR)
	$(TOOLCHAINS_PATH)/bin/$(GCC) $(CFLAGS) -c $< -o $@

# Compile STL source files
$(STL_BUILD_DIR)/%.o: $(STL_SRC_DIR)/%.c
	@mkdir -p $(STL_BUILD_DIR)
	$(TOOLCHAINS_PATH)/bin/$(GCC) $(CFLAGS) -c $< -o $@

# Compile main.c
$(BUILD_DIR)/main.o: main.c
	@mkdir -p $(BUILD_DIR)
	$(TOOLCHAINS_PATH)/bin/$(GCC) $(CFLAGS) -c $< -o $@

# Assemble startup file
$(STARTUP_OBJECT): $(STARTUP_DIR)/$(STARTUP_FILE)
	@mkdir -p $(BUILD_DIR)
	$(TOOLCHAINS_PATH)/bin/$(GCC) $(CFLAGS) -c $< -o $@

# Link to create ELF file
$(BUILD_DIR)/$(PROJECT).elf: $(BUILD_DIR)/main.o $(USER_OBJECTS) $(CMSIS_OBJECTS) $(STL_OBJECTS) $(STARTUP_OBJECT)
	$(TOOLCHAINS_PATH)/bin/$(GCC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	@echo "Build complete: $@"

# Create BIN file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	$(TOOLCHAINS_PATH)/bin/$(CP) -O binary $< $@
	@echo "Created: $@"

# Create HEX file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	$(TOOLCHAINS_PATH)/bin/$(CP) -O ihex $< $@
	@echo "Created: $@"

# Clean build files
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# Flash to STM32
flash: $(BUILD_DIR)/$(PROJECT).bin
	st-flash write $< 0x8000000
	@echo "Flash complete"

# Debug
debug: 
	gdb-multiarch build/main.elf \
    -ex "target extended-remote localhost:3333" \
    -ex "monitor reset halt" \
    -ex "load" \
    -ex "break main" \
    -ex "continue"
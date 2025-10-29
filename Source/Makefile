# =====================
# MCU & Toolchain
# =====================
MCU = stm32f103c8t6
CPU = cortex-m3
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# =====================
# Flags & Include Paths
# =====================
CFLAGS = -mcpu=$(CPU) -mthumb -Wall -O2 -ffunction-sections -fdata-sections\
		-IInc \
		-IDrivers/STM32F10x_StdPeriph_Driver/inc \
		-ICMSIS/CM3/DeviceSupport/ST/STM32F10x \
		-ICMSIS/CM3/CoreSupport \
		-DCMSIS -DSTM32F10X_MD \
		-include Drivers/STM32F10x_StdPeriph_Driver/inc/stm32f10x_conf.h

LDFLAGS = -TSTM32F103C8TX_FLASH.ld -nostartfiles -Wl,-Map=$(TARGET:.elf=.map) -Wl,--gc-sections --specs=nosys.specs --specs=nano.specs -u _printf_float -lnosys

# =====================
# Source Files
# =====================
C_SRC = $(wildcard Src/*.c) \
		Drivers/STM32F10x_StdPeriph_Driver/src/misc.c \
		CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c

S_SRC = Src/startup_stm32f10x_md.s

# =====================
# Object Output Path
# =====================
OBJ_DIR = build/obj

# Redirect all object files (.o) into build/obj/
C_OBJ = $(C_SRC:%.c=$(OBJ_DIR)/%.o)
S_OBJ = $(S_SRC:%.s=$(OBJ_DIR)/%.o)
OBJ = $(C_OBJ) $(S_OBJ)

TARGET = build/main.elf

# =====================
# Build Rules
# =====================
all: $(TARGET)

# Debug build flags
CFLAGS_DEBUG = $(CFLAGS) -O0 -g

# Link
$(TARGET): $(OBJ)
	mkdir -p build
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Compile C
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly
$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	@echo "Cleaning build files..."
	@chmod -R u+w build || true
	@rm -rf $(OBJ_DIR) $(TARGET) build/main.bin build/main.hex

# Convert ELF -> HEX
hex: $(TARGET)
	$(OBJCOPY) -O ihex $(TARGET) build/main.hex

# =====================
# Flash
# =====================
flash: hex
	@echo "Flashing with ST-LINK CLI on Windows..."
	@HEX_WIN_PATH=$$(wslpath -w build/main.hex); \
	cmd.exe /C ST-LINK_CLI.exe" -c SWD -P "$$HEX_WIN_PATH" 0x08000000 -V -Rst"

# =====================
# Debug target (build + flash)
# =====================
debug: CFLAGS := $(CFLAGS_DEBUG)
debug: clean $(TARGET)
	@echo "Debug build complete. Flashing MCU..."
	@$(MAKE) flash
	@echo "Debug build flashed. Use arm-none-eabi-gdb with your debug.gdb script."
	@echo "Starting Windows PowerShell to run debug.bat..."
	@set -e; \
	DEBUG_BAT=$$(wslpath -w "$$PWD")\\debug.bat; \
	echo "DEBUG_BAT = $$DEBUG_BAT"; \
	powershell.exe -Command "Start-Process powershell -ArgumentList '-NoExit','-ExecutionPolicy Bypass','-Command','& \"$$DEBUG_BAT\"'"

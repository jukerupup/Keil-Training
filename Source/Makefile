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
CFLAGS = -mcpu=$(CPU) -mthumb -Wall -O2 -ffunction-sections -fdata-sections
CFLAGS += -IInc \
          -IDrivers/STM32F10x_StdPeriph_Driver/inc \
          -ICMSIS/CM3/DeviceSupport/ST/STM32F10x \
          -ICMSIS/CM3/CoreSupport \
          -DCMSIS -DSTM32F10X_MD \
          -include Drivers/STM32F10x_StdPeriph_Driver/inc/stm32f10x_conf.h

LDFLAGS = -TSTM32F103C8TX_FLASH.ld -nostartfiles -Wl,--gc-sections

# =====================
# Source Files
# =====================
C_SRC = $(wildcard Src/*.c) \
        Drivers/STM32F10x_StdPeriph_Driver/src/misc.c \
        CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c

S_SRC = Src/startup_stm32f10x_md.s

C_OBJ = $(C_SRC:.c=.o)
S_OBJ = $(S_SRC:.s=.o)
OBJ = $(C_OBJ) $(S_OBJ)

TARGET = build/main.elf

# =====================
# Build Rules
# =====================
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	mkdir -p build
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Compile C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET) build/main.bin build/main.hex

# Convert ELF -> HEX
hex: $(TARGET)
	$(OBJCOPY) -O ihex $(TARGET) build/main.hex


flash: hex
	@echo "Flashing with ST-LINK CLI on Windows..."
	@HEX_WIN_PATH=$$(wslpath -w build/main.hex); \
	/mnt/c/Windows/System32/cmd.exe /C ST-LINK_CLI.exe" -c SWD -P "$$HEX_WIN_PATH" 0x08000000 -V -Rst"

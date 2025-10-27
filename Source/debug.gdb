# debug.gdb
# Connect to OpenOCD GDB server
#cd \Keil-Training\Source
#PowerShell :arm-none-eabi-gdb -x debug.gdb build/main.elf
target remote localhost:3333

# Reset the MCU and halt at reset handler
monitor reset halt

# Load the firmware to the MCU
load

# Initialize the MCU
monitor reset init

# Set a breakpoint at main()
break main

# Continue execution until main()
continue

# Optional commands to inspect MCU state
# info registers
# bt
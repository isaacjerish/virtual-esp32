XTENSA_PREFIX = xtensa-esp32-elf-
CC = $(XTENSA_PREFIX)gcc
AS = $(XTENSA_PREFIX)as
LD = $(XTENSA_PREFIX)ld
OBJCOPY = $(XTENSA_PREFIX)objcopy
SIZE = $(XTENSA_PREFIX)size
CFLAGS = -std=c11 -Wall -Wextra -O2 -g
CFLAGS += -mtext-section-literals -fno-builtin
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -D__XTENSA__ -DXTENSA
ASFLAGS = --32
LDFLAGS = -nostdlib -nostartfiles
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(TARGET).map
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TARGET = $(BINDIR)/firmware
SOURCES = $(wildcard $(SRCDIR)/*.c)
ASMSOURCES = $(wildcard $(SRCDIR)/*.s)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ASMOBJECTS = $(ASMSOURCES:$(SRCDIR)/%.s=$(OBJDIR)/%.o)
ALLOBJECTS = $(OBJECTS) $(ASMOBJECTS)
all: $(TARGET).bin
$(OBJDIR):
	mkdir -p $(OBJDIR)
$(BINDIR):
	mkdir -p $(BINDIR)
$(TARGET).bin: $(TARGET).elf | $(BINDIR)
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<
$(TARGET).elf: $(ALLOBJECTS) $(SRCDIR)/startup.s | $(BINDIR)
	$(LD) $(LDFLAGS) -T $(SRCDIR)/linker.ld -o $@ $^
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJDIR)/%.o: $(SRCDIR)/%.s | $(OBJDIR)
	$(AS) $(ASFLAGS) $< -o $@
clean:
	rm -rf $(OBJDIR) $(BINDIR)
flash: $(TARGET).bin
	cp $(TARGET).bin ../firmware.bin
	@echo "Firmware copied to ../firmware.bin"
run: flash
	cd .. && make run
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET).bin
release: CFLAGS += -DNDEBUG -O3
release: $(TARGET).bin
size: $(TARGET).elf
	$(SIZE) $<
disasm: $(TARGET).elf
	$(XTENSA_PREFIX)objdump -d $<
help:
	@echo "Available targets:"
	@echo "  all       - Build firmware binary (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  flash     - Copy binary to emulator directory"
	@echo "  run       - Build and run in emulator"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized release version"
	@echo "  size      - Show firmware size"
	@echo "  disasm    - Disassemble firmware"
	@echo "  help      - Show this help message"

.PHONY: all clean flash run debug release size disasm help 
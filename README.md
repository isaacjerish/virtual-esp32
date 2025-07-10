# Virtual ESP32 Emulator (vesp)

So basically I got tired of flashing firmware to real ESP32s every time I wanted to test something, so I wrote this emulator in C++. 

## What it does

This thing simulates an ESP32 SoC and can run firmware binaries that you compile with the usual Xtensa toolchain. Here's what I've got working so far:

- **Xtensa LX6 CPU**: Only implements the basic instructions I needed (L32I.N, S32I.N, ADD.N, JMP). There's definitely more that could be added
- **4MB RAM**: Simulates the ESP32's memory layout, kinda
- **UART**: So you can actually see printf output and stuff
- **WiFi**: This one was tricky but I got basic HTTP requests working
- **CLI tool**: Just run `vesp firmware.bin` and it works

## How it's organized

```
virtual-esp32/
├── src/                    # Main emulator code
│   ├── main.cpp           # Where the magic starts
│   ├── Emulator.{h,cpp}   # Ties everything together
│   ├── XtensaLX6.{h,cpp}  # CPU emulation (the hard part)
│   ├── Memory.{h,cpp}     # Memory management
│   └── peripherals/       # All the peripheral stuff
│       ├── Peripheral.{h,cpp}  # Base class for peripherals
│       ├── UART.{h,cpp}        # Serial communication
│       └── WiFi.{h,cpp}        # WiFi simulation
├── firmware/              # Test firmware
│   ├── src/              # Firmware source
│   │   ├── main.c        # Simple test program
│   │   ├── startup.s     # Assembly startup code
│   │   └── linker.ld     # Linker script (copied from ESP-IDF)
│   └── Makefile          # Build system
├── Makefile              # Emulator build
└── README.md            # This file
```

## What you need

### For the emulator itself
- **C++20 compiler** - I use GCC but clang should work too
- **Make** - for building
- **Optional stuff**: clang-format if you care about code style

### For building firmware
- **Xtensa ESP32 toolchain** - this is the tricky part to install
- **Make** - same as above

### Getting the Xtensa toolchain

#### macOS
```bash
# If you have Homebrew (easiest way)
brew install esp32-toolchain

# Otherwise download from Espressif's site
# https://dl.espressif.com/dl/xtensa-esp32-elf-gcc8_4_0-esp-2021r2-patch5-osx.tar.gz
```

#### Linux
```bash
# Download and extract - this URL might be out of date
wget https://dl.espressif.com/dl/xtensa-esp32-elf-gcc8_4_0-esp-2021r2-patch5-linux-amd64.tar.gz
tar -xzf xtensa-esp32-elf-gcc8_4_0-esp-2021r2-patch5-linux-amd64.tar.gz
export PATH=$PATH:./xtensa-esp32-elf/bin
```

#### Windows
Download from Espressif and add to PATH. I don't use Windows much so YMMV.

## Building everything

### Building the emulator

```bash
# Just build it
make

# Debug build (slower but easier to debug)
make debug

# Release build (faster)
make release

# Clean up
make clean
```

You'll get a binary at `bin/vesp`.

### Building test firmware

```bash
# Go to firmware directory
cd firmware

# Build the firmware
make

# Copy firmware to main directory
make flash

# Build and run in one step
make run
```

This creates `firmware/bin/firmware.bin`.

## How to use it

### Basic usage

```bash
# Run with a firmware binary
./bin/vesp firmware.bin

# Or if you installed it somewhere
vesp firmware.bin
```

### Example workflow

1. **Build the emulator:**
   ```bash
   make
   ```

2. **Build the test firmware:**
   ```bash
   cd firmware
   make
   make flash
   cd ..
   ```

3. **Run it:**
   ```bash
   ./bin/vesp firmware.bin
   ```

You should see something like:
```
ESP32 Emulator initialized
Memory initialized: 4MB RAM
RAM range: 0x3ff80000 - 0x3fffffff
Xtensa LX6 CPU initialized
UART peripheral initialized at 0x3ff40000
Loaded firmware: firmware.bin (1234 bytes)
Starting emulation...
Hello from ESP32 firmware!
Count: 0
Count: 1
Count: 2
Count: 3
Count: 4
Firmware execution complete.
```

## Development phases

### Phase 1: Basic CPU + RAM ✅
- [x] Got basic Xtensa CPU working
- [x] 4MB RAM simulation
- [x] Basic instruction set (just what I needed)
- [x] Loading and running firmware

### Phase 2: UART ✅
- [x] UART peripheral at 0x3FF40000
- [x] Serial output works
- [x] Hello world example

### Phase 3: WiFi ✅
- [x] WiFi peripheral simulation
- [x] HTTP requests (kinda hacky but works)
- [x] Connects to host network

## Memory layout

```
0x40080000 - 0x3FFFFFFF: 4MB RAM (might have this backwards?)
0x3FF40000 - 0x3FF400FF: UART
0x3FF50000 - 0x3FF500FF: WiFi
```

## Supported instructions

Only got the basics working so far:

- **L32I.N**: Load 32-bit from memory
- **S32I.N**: Store 32-bit to memory  
- **ADD.N**: Add two registers
- **JMP**: Jump to address

There's definitely more instructions that could be added but these cover most of what I needed.

## Peripheral system

All peripherals inherit from a base `Peripheral` class and get memory-mapped I/O. Pretty straightforward.

### UART

Lives at `0x3FF40000`:
- **0x00**: Data register (for TX/RX)
- **0x04**: Status register
- **0x08**: Control register  
- **0x0C**: Baud rate (doesn't really matter in emulation)

### WiFi

At `0x3FF50000`:
- **0x00**: Control register
- **0x04**: Status register
- **0x08**: Data register
- **0x0C**: Address register
- **0x10**: Response register

## Adding more peripherals

Pretty easy to add new ones:

1. Make a new class that inherits from `Peripheral`
2. Implement the read/write methods
3. Add it to the emulator in `Emulator.cpp`

Something like:
```cpp
class MyPeripheral : public Peripheral {
public:
    MyPeripheral() : Peripheral(0x3FF60000, 0x100) {}
    
    uint8_t read8(uint32_t offset) const override {
        // Do something
    }
    
    void write8(uint32_t offset, uint8_t value) override {
        // Do something else
    }
    
    // ... other methods
};
```

### Debugging

```bash
# Build with debug info
make debug

# Run with valgrind (if you have it)
make memcheck

# Static analysis
make analyze

# Format code
make format
```

## Testing

Right now it's mostly manual testing - just build the example firmware and make sure it runs. Should probably add some automated tests at some point.

## Performance

Don't expect this to be fast. It's meant for development and testing, not speed. On my machine it does maybe 100K instructions per second, which is fine for most testing.

## What's missing / broken

- Only supports a tiny subset of Xtensa instructions
- No floating point at all
- No interrupts (this is a big one)
- Peripheral implementations are pretty basic
- Probably has bugs I haven't found yet
- Memory layout might not be 100% accurate

## Contributing

If you want to help out:
1. Fork it
2. Make your changes
3. Test it
4. Send a pull request

I'm pretty informal about this stuff.

## License

It's open source, do whatever you want with it. 

## Thanks

- Espressif for making the ESP32 and documenting it
- Whoever wrote the Xtensa architecture docs
- ESP-IDF project for examples I could copy from

## Getting help

If something doesn't work:
1. Check if you have all the dependencies
2. Make sure your firmware actually works on real hardware first
3. File an issue if you think it's a bug in the emulator

---

**Note**: This is just a development tool I wrote for myself. It's not meant to replace real ESP32 hardware or be used in production. Use at your own risk!
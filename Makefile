
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -g
INCLUDES = -Isrc -Isrc/peripherals

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp) \
          $(wildcard $(SRCDIR)/peripherals/*.cpp)

OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

TARGET = $(BINDIR)/vesp

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/peripherals

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/


uninstall:
	sudo rm -f /usr/local/bin/vesp

run: $(TARGET)
	@if [ -f firmware.bin ]; then \
		./$(TARGET) firmware.bin; \
	else \
		echo "No firmware.bin found. Create a firmware binary first."; \
	fi

debug: CXXFLAGS += -DDEBUG -O0
debug: $(TARGET)

release: CXXFLAGS += -DNDEBUG -O3
release: $(TARGET)

memcheck: $(TARGET)
	@if [ -f firmware.bin ]; then \
		valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) firmware.bin; \
	else \
		echo "No firmware.bin found. Create a firmware binary first."; \
	fi



help:
	@echo "Available targets:"
	@echo "  all       - Build the emulator (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized release version"
	@echo "  run       - Run with firmware.bin (if available)"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  memcheck  - Run with valgrind memory checking"
	@echo "  format    - Format code with clang-format"
	@echo "  analyze   - Run static analysis with cppcheck"
	@echo "  help      - Show this help message"

.PHONY: all clean debug release run install uninstall memcheck  help 
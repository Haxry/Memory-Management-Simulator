# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -g

# Project structure
SRCDIR := src
INCDIR := include
OBJDIR := build
ALLOCDIR := $(SRCDIR)/allocator
CACHEDIR := $(SRCDIR)/cache

# Target executable
TARGET := memory_simulator

# Source files
MAIN_SRC := $(SRCDIR)/main.cpp
CMD_SRC := $(SRCDIR)/command_processor.cpp  
ALLOC_SRC := $(ALLOCDIR)/memory_manager.cpp
CACHE_SRC := $(CACHEDIR)/cache_simulator.cpp

# Object files
MAIN_OBJ := $(OBJDIR)/main.o
CMD_OBJ := $(OBJDIR)/command_processor.o
ALLOC_OBJ := $(OBJDIR)/memory_manager.o
CACHE_OBJ := $(OBJDIR)/cache_simulator.o

OBJECTS := $(MAIN_OBJ) $(CMD_OBJ) $(ALLOC_OBJ) $(CACHE_OBJ)

# Include path
INCLUDES := -I$(INCDIR)

# Default target
.PHONY: all clean help run test

all: $(TARGET)

# Create build directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build target executable
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
	@echo "Build completed successfully!"
	@echo "Run with: ./$(TARGET)"

# Compile source files
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(MAIN_SRC) -o $(MAIN_OBJ)

$(CMD_OBJ): $(CMD_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(CMD_SRC) -o $(CMD_OBJ)

$(ALLOC_OBJ): $(ALLOC_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ALLOC_SRC) -o $(ALLOC_OBJ)

$(CACHE_OBJ): $(CACHE_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(CACHE_SRC) -o $(CACHE_OBJ)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run basic tests
test: $(TARGET)
	@echo "Running basic functionality test..."
	@echo -e "init 1024\nstrategy first_fit\nalloc 200\nalloc 300\nfree 1\ndisplay\nstats\nexit" | ./$(TARGET)

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Cleaned build files"

# Display help information
help:
	@echo "Available targets:"
	@echo "  all     - Build the memory simulator (default)"
	@echo "  run     - Build and run the simulator"
	@echo "  test    - Run basic functionality test"
	@echo "  clean   - Remove build files and executable"
	@echo "  help    - Display this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  make          # Build the project"
	@echo "  make run      # Build and run interactively"
	@echo "  make test     # Run automated test"
	@echo "  make clean    # Clean build artifacts"

# Debugging target with verbose output
debug: CXXFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)
	@echo "Debug build completed"

# Dependency tracking (basic)
$(MAIN_OBJ): $(INCDIR)/memory_manager.hpp $(INCDIR)/command_processor.hpp $(INCDIR)/cache_simulator.hpp
$(CMD_OBJ): $(INCDIR)/command_processor.hpp $(INCDIR)/memory_manager.hpp
$(ALLOC_OBJ): $(INCDIR)/memory_manager.hpp  
$(CACHE_OBJ): $(INCDIR)/cache_simulator.hpp

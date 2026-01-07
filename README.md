# Advanced Memory Management Simulator v2.0

A comprehensive simulation system for physical memory management algorithms and multi-level cache hierarchies, designed for educational and research purposes in computer systems.

## 🚀 Features

### Memory Management
- **Dynamic Memory Allocation**: Support for First Fit, Best Fit, and Worst Fit strategies
- **Intelligent Block Management**: Automatic block splitting and coalescing
- **Real-time Visualization**: Interactive memory layout display
- **Performance Analytics**: Comprehensive fragmentation and utilization statistics

### Cache Simulation
- **Multi-level Cache Hierarchy**: L1 and L2 cache simulation
- **FIFO Replacement Policy**: First-In-First-Out cache eviction strategy  
- **Configurable Architecture**: Customizable cache sizes and block configurations
- **Performance Tracking**: Hit/miss ratios and access pattern analysis

### Interactive Interface
- **Command-line Interface**: Intuitive text-based interaction
- **Flexible Command Syntax**: Multiple command aliases for user convenience
- **Real-time Feedback**: Immediate response to allocation requests
- **Comprehensive Help System**: Built-in documentation and usage examples

## 🏗️ Project Structure

```
memory-simulator/
├── src/                          # Source code directory
│   ├── main.cpp                  # Application entry point
│   ├── command_processor.cpp     # Command interface implementation
│   ├── allocator/                # Memory allocation modules
│   │   └── memory_manager.cpp    # Core memory management logic
│   └── cache/                    # Cache simulation modules
│       └── cache_simulator.cpp   # Multi-level cache implementation
├── include/                      # Header files
│   ├── memory_manager.hpp        # Memory management interfaces
│   ├── command_processor.hpp     # Command processing interfaces
│   └── cache_simulator.hpp       # Cache simulation interfaces
├── tests/                        # Test cases and validation
│   ├── test_basic.txt           # Basic functionality tests
│   └── expected_output_basic.txt # Expected test results
├── docs/                         # Documentation
├── demo/                         # Demonstration materials
└── Makefile                      # Build configuration
```

## 🔧 Build Instructions

### Prerequisites
- C++17 compatible compiler (GCC 7+ or Clang 5+)
- Make utility

### Compilation
```bash
# Build the simulator
make

# Build and run immediately
make run

# Build with debug symbols
make debug

# Run automated tests
make test

# Clean build artifacts
make clean
```

## 💡 Usage Guide

### Starting the Simulator
```bash
./memory_simulator
```

### Core Commands

#### Memory Management
```bash
# Initialize memory pool
init <size>                    # Example: init 1024

# Set allocation strategy  
strategy <algorithm>           # Options: first_fit, best_fit, worst_fit

# Allocate memory block
alloc <size>                   # Example: alloc 256

# Deallocate memory block  
free <process_id>              # Example: free 3

# Display current memory layout
display

# Show comprehensive statistics
stats
```

#### System Control
```bash
# Reset entire simulator
reset

# Display help information  
help

# Exit simulator
exit
```

### Example Session
```bash
memsim> init 1024
Memory pool initialized: 1024 bytes

memsim> strategy best_fit  
Allocation strategy set to: Best Fit

memsim> alloc 200
Memory allocated: PID=1 at address=0x0 (size=200)

memsim> alloc 150
Memory allocated: PID=2 at address=0xc8 (size=150)

memsim> free 1
Memory deallocated for PID=1

memsim> display
--- Current Memory Layout ---
[0x0 - 0xc7] FREE (size=200)
[0xc8 - 0x15d] ALLOCATED (PID=2, size=150)  
[0x15e - 0x3ff] FREE (size=674)
-----------------------------

memsim> stats
--- Memory Analysis Report ---
Total memory capacity: 1024 bytes
Allocated memory: 150 bytes
Free memory: 874 bytes
Memory utilization: 14.65%
External fragmentation: 22.88%
Internal fragmentation: 0.00% (exact allocation)
-----------------------------
```

## 📊 Performance Metrics

The simulator tracks comprehensive performance statistics:

- **Memory Utilization**: Percentage of allocated vs. total memory
- **External Fragmentation**: Wasted space due to non-contiguous free blocks  
- **Internal Fragmentation**: Always 0% due to exact block splitting
- **Allocation Success Rate**: Ratio of successful to total allocation attempts
- **Cache Performance**: Hit/miss ratios across cache hierarchy levels

## 🔬 Technical Implementation

### Memory Management Algorithms

**First Fit**: Allocates memory in the first suitable free block found during linear search.

**Best Fit**: Selects the smallest free block that can accommodate the request, minimizing wasted space.

**Worst Fit**: Chooses the largest available free block, potentially reducing future external fragmentation.

### Cache Architecture

The simulator implements a realistic two-level cache hierarchy with:
- Configurable cache sizes and block sizes
- FIFO replacement policy for cache eviction
- Accurate hit/miss tracking and propagation between levels
- Support for direct-mapped and set-associative configurations

### Block Management

- **Dynamic Splitting**: Automatically divides large free blocks when smaller allocations are requested
- **Intelligent Coalescing**: Merges adjacent free blocks during deallocation to reduce fragmentation
- **Address Translation**: Uses realistic hexadecimal memory addressing for authentic simulation


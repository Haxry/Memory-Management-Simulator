# Memory Management Simulator Documentation

## Architecture Overview

The Advanced Memory Management Simulator is built with a modular architecture that separates concerns and promotes code reusability.

## Core Components

### 1. PhysicalMemorySimulator Class
Located in `src/allocator/memory_manager.cpp`

**Purpose**: Manages the simulation of physical memory allocation and deallocation.

**Key Features**:
- Dynamic memory pool initialization
- Three allocation strategies (First Fit, Best Fit, Worst Fit)
- Automatic block splitting and coalescing
- Comprehensive statistics tracking

**Major Methods**:
- `initialize_memory_pool(size_t total_size)`: Sets up the memory simulation
- `allocate_memory_block(size_t requested_size)`: Handles memory allocation requests
- `deallocate_memory_block(int process_id)`: Manages memory deallocation
- `display_fragmentation_analysis()`: Provides detailed memory statistics

### 2. CommandLineProcessor Class
Located in `src/command_processor.cpp`

**Purpose**: Provides an interactive command-line interface for user interaction.

**Key Features**:
- Flexible command parsing with multiple aliases
- Robust error handling and input validation
- Interactive help system
- Command history and session management

**Supported Commands**:
- Memory operations: `init`, `alloc`, `free`
- Configuration: `strategy`  
- Information: `display`, `stats`, `help`
- Control: `reset`, `exit`

### 3. MultiLevelCacheSimulator Class
Located in `src/cache/cache_simulator.cpp`

**Purpose**: Simulates a realistic multi-level cache hierarchy.

**Key Features**:
- L1 and L2 cache simulation
- FIFO replacement policy
- Configurable cache parameters
- Performance metrics tracking

**Cache Operations**:
- Memory access simulation with hit/miss tracking
- Automatic cache eviction using FIFO policy
- Cache flush and reset capabilities
- Detailed performance reporting

## Memory Management Algorithms

### First Fit Algorithm
```cpp
int find_suitable_segment_first_fit(size_t required_size) {
    for (size_t i = 0; i < heap_segments.size(); ++i) {
        if (heap_segments[i].can_accommodate(required_size)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
```

**Characteristics**:
- Fast allocation (O(n) worst case, often faster in practice)
- Simple implementation
- Can lead to external fragmentation at the beginning of memory

### Best Fit Algorithm
```cpp
int find_suitable_segment_best_fit(size_t required_size) {
    int best_index = -1;
    size_t smallest_suitable_size = SIZE_MAX;
    
    for (size_t i = 0; i < heap_segments.size(); ++i) {
        if (heap_segments[i].can_accommodate(required_size)) {
            if (heap_segments[i].segment_size < smallest_suitable_size) {
                smallest_suitable_size = heap_segments[i].segment_size;
                best_index = static_cast<int>(i);
            }
        }
    }
    return best_index;
}
```

**Characteristics**:
- Minimizes wasted space per allocation
- O(n) time complexity for each allocation
- Can create many small unusable fragments

### Worst Fit Algorithm
```cpp
int find_suitable_segment_worst_fit(size_t required_size) {
    int worst_index = -1;
    size_t largest_suitable_size = 0;
    
    for (size_t i = 0; i < heap_segments.size(); ++i) {
        if (heap_segments[i].can_accommodate(required_size)) {
            if (heap_segments[i].segment_size > largest_suitable_size) {
                largest_suitable_size = heap_segments[i].segment_size;
                worst_index = static_cast<int>(i);
            }
        }
    }
    return worst_index;
}
```

**Characteristics**:
- Attempts to leave large fragments for future allocations
- O(n) time complexity
- Can lead to faster external fragmentation growth

## Cache Simulation Details

### Cache Architecture
The simulator implements a realistic two-level cache hierarchy:

```
CPU -> L1 Cache -> L2 Cache -> Main Memory
```

### FIFO Replacement Policy
When a cache is full and a new block needs to be loaded:

1. The oldest block (first in FIFO queue) is evicted
2. New block is loaded into the freed location
3. New block is added to the end of FIFO queue

### Cache Access Simulation
```cpp
bool access_memory(size_t memory_address) {
    size_t block_index = calculate_block_index(memory_address);
    size_t tag = extract_tag(memory_address);
    
    // Check for cache hit
    if (cache_blocks[block_index].valid && cache_blocks[block_index].tag == tag) {
        performance_metrics.record_hit();
        return true; // Cache hit
    }
    
    // Cache miss handling...
    performance_metrics.record_miss();
    // Load new block and update FIFO queue
    return false; // Cache miss
}
```

## Performance Metrics

### Memory Utilization
```
Utilization = (Allocated Memory / Total Memory) * 100%
```

### External Fragmentation
```
External Fragmentation = ((Total Free - Largest Free Block) / Total Free) * 100%
```

### Cache Hit Ratio
```
Hit Ratio = (Cache Hits / Total Accesses) * 100%
```

## Error Handling

The simulator implements comprehensive error handling:

- **Input Validation**: All user inputs are validated for format and range
- **State Checking**: Operations verify system state before execution  
- **Resource Management**: RAII principles ensure proper resource cleanup
- **Exception Safety**: All operations are exception-safe with proper rollback

## Extension Points

The modular architecture allows for easy extension:

1. **New Allocation Algorithms**: Implement new strategies by extending the allocation interface
2. **Additional Cache Levels**: Add L3 or instruction/data split caches
3. **Different Replacement Policies**: Implement LRU, LFU, or random replacement
4. **Virtual Memory**: Add page table simulation and address translation
5. **Buddy Allocation**: Implement buddy system memory management

## Testing Strategy

The simulator includes comprehensive testing:

- **Unit Tests**: Individual component testing
- **Integration Tests**: Multi-component interaction validation
- **Performance Tests**: Stress testing with large memory pools
- **Regression Tests**: Ensure new features don't break existing functionality

## Build System

The Makefile supports multiple build configurations:

- **Release Build**: Optimized for performance (`make`)
- **Debug Build**: Includes debug symbols (`make debug`)
- **Test Build**: Runs automated test suite (`make test`)
- **Clean Build**: Removes all build artifacts (`make clean`)

This documentation provides a comprehensive overview of the simulator's architecture and implementation details for developers and researchers working with the system.

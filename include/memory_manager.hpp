#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <vector>
#include <iostream>
#include <climits>
#include <iomanip>

/**
 * @brief Memory allocation strategies supported by the simulator
 */
enum class AllocationStrategy {
    FIRST_FIT,
    BEST_FIT, 
    WORST_FIT
};

/**
 * @brief Represents a memory segment in the simulated heap
 */
class MemorySegment {
public:
    size_t base_address;      // Starting memory address
    size_t segment_size;      // Size of this memory segment
    bool is_available;        // True if free, false if allocated
    int process_id;          // ID of allocated process (-1 if free)
    
    MemorySegment(size_t addr, size_t size, bool available = true, int pid = -1);
    
    // Utility methods
    size_t end_address() const { return base_address + segment_size - 1; }
    bool can_accommodate(size_t required_size) const;
    void display_info() const;
};

/**
 * @brief Statistics tracker for memory operations
 */
struct MemoryStatistics {
    int total_allocation_attempts;
    int successful_allocations;
    int failed_allocations;
    
    MemoryStatistics() : total_allocation_attempts(0), 
                        successful_allocations(0), 
                        failed_allocations(0) {}
    
    void record_allocation_attempt() { total_allocation_attempts++; }
    void record_successful_allocation() { successful_allocations++; }
    void record_failed_allocation() { failed_allocations++; }
    
    double get_success_rate() const;
    void display_statistics() const;
};

/**
 * @brief Main memory management simulator class
 */
class PhysicalMemorySimulator {
private:
    std::vector<MemorySegment> heap_segments;
    size_t total_memory_capacity;
    int next_process_identifier;
    AllocationStrategy current_strategy;
    MemoryStatistics performance_stats;
    
    // Private helper methods
    int find_suitable_segment_first_fit(size_t required_size);
    int find_suitable_segment_best_fit(size_t required_size);
    int find_suitable_segment_worst_fit(size_t required_size);
    void split_memory_segment(int segment_index, size_t allocation_size);
    void merge_adjacent_free_segments();
    void merge_with_next_segment(int current_index);
    void merge_with_previous_segment(int current_index);
    
public:
    PhysicalMemorySimulator();
    ~PhysicalMemorySimulator() = default;
    
    // Core functionality
    void initialize_memory_pool(size_t total_size);
    void set_allocation_strategy(AllocationStrategy strategy);
    int allocate_memory_block(size_t requested_size);
    bool deallocate_memory_block(int process_id);
    
    // Information and diagnostics
    void display_memory_layout() const;
    void display_fragmentation_analysis() const;
    void reset_simulator();
    
    // Getters
    size_t get_total_memory() const { return total_memory_capacity; }
    AllocationStrategy get_current_strategy() const { return current_strategy; }
    const MemoryStatistics& get_statistics() const { return performance_stats; }
};

#endif // MEMORY_MANAGER_HPP

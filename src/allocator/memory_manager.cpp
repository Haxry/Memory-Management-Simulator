#include "memory_manager.hpp"
#include <algorithm>

// MemorySegment implementation
MemorySegment::MemorySegment(size_t addr, size_t size, bool available, int pid)
    : base_address(addr), segment_size(size), is_available(available), process_id(pid) {}

bool MemorySegment::can_accommodate(size_t required_size) const {
    return is_available && segment_size >= required_size;
}

void MemorySegment::display_info() const {
    std::cout << "[0x" << std::hex << base_address 
              << " - 0x" << end_address() << std::dec << "] ";
    
    if (is_available) {
        std::cout << "FREE (size=" << segment_size << ")\n";
    } else {
        std::cout << "ALLOCATED (PID=" << process_id << ", size=" << segment_size << ")\n";
    }
}

// MemoryStatistics implementation
double MemoryStatistics::get_success_rate() const {
    if (total_allocation_attempts == 0) return 0.0;
    return (100.0 * successful_allocations) / total_allocation_attempts;
}

void MemoryStatistics::display_statistics() const {
    std::cout << "\n=== Memory Performance Statistics ===\n";
    std::cout << "Total allocation requests: " << total_allocation_attempts << "\n";
    std::cout << "Successful allocations: " << successful_allocations << "\n";
    std::cout << "Failed allocations: " << failed_allocations << "\n";
    std::cout << "Success rate: " << std::fixed << std::setprecision(2) 
              << get_success_rate() << "%\n";
    std::cout << "====================================\n";
}

// PhysicalMemorySimulator implementation
PhysicalMemorySimulator::PhysicalMemorySimulator() 
    : total_memory_capacity(0), next_process_identifier(1), 
      current_strategy(AllocationStrategy::FIRST_FIT) {}

void PhysicalMemorySimulator::initialize_memory_pool(size_t total_size) {
    total_memory_capacity = total_size;
    heap_segments.clear();
    heap_segments.emplace_back(0, total_size, true, -1);
    next_process_identifier = 1;
    performance_stats = MemoryStatistics();
    
    std::cout << "Memory pool initialized: " << total_size << " bytes\n";
}

void PhysicalMemorySimulator::set_allocation_strategy(AllocationStrategy strategy) {
    current_strategy = strategy;
    
    std::string strategy_name;
    switch (strategy) {
        case AllocationStrategy::FIRST_FIT: strategy_name = "First Fit"; break;
        case AllocationStrategy::BEST_FIT: strategy_name = "Best Fit"; break;
        case AllocationStrategy::WORST_FIT: strategy_name = "Worst Fit"; break;
    }
    
    std::cout << "Allocation strategy set to: " << strategy_name << "\n";
}

int PhysicalMemorySimulator::allocate_memory_block(size_t requested_size) {
    performance_stats.record_allocation_attempt();
    
    if (requested_size == 0) {
        std::cout << "Error: Cannot allocate zero bytes\n";
        performance_stats.record_failed_allocation();
        return -1;
    }
    
    int segment_index = -1;
    
    switch (current_strategy) {
        case AllocationStrategy::FIRST_FIT:
            segment_index = find_suitable_segment_first_fit(requested_size);
            break;
        case AllocationStrategy::BEST_FIT:
            segment_index = find_suitable_segment_best_fit(requested_size);
            break;
        case AllocationStrategy::WORST_FIT:
            segment_index = find_suitable_segment_worst_fit(requested_size);
            break;
    }
    
    if (segment_index == -1) {
        std::cout << "Memory allocation failed: Insufficient space\n";
        performance_stats.record_failed_allocation();
        return -1;
    }
    
    // Perform the allocation
    size_t allocated_address = heap_segments[segment_index].base_address;
    int assigned_pid = next_process_identifier++;
    
    split_memory_segment(segment_index, requested_size);
    
    heap_segments[segment_index].is_available = false;
    heap_segments[segment_index].process_id = assigned_pid;
    
    performance_stats.record_successful_allocation();
    
    std::cout << "Memory allocated: PID=" << assigned_pid 
              << " at address=0x" << std::hex << allocated_address 
              << std::dec << " (size=" << requested_size << ")\n";
    
    return assigned_pid;
}

bool PhysicalMemorySimulator::deallocate_memory_block(int process_id) {
    for (auto& segment : heap_segments) {
        if (!segment.is_available && segment.process_id == process_id) {
            segment.is_available = true;
            segment.process_id = -1;
            
            merge_adjacent_free_segments();
            
            std::cout << "Memory deallocated for PID=" << process_id << "\n";
            return true;
        }
    }
    
    std::cout << "Error: Process ID " << process_id << " not found\n";
    return false;
}

int PhysicalMemorySimulator::find_suitable_segment_first_fit(size_t required_size) {
    for (size_t i = 0; i < heap_segments.size(); ++i) {
        if (heap_segments[i].can_accommodate(required_size)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int PhysicalMemorySimulator::find_suitable_segment_best_fit(size_t required_size) {
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

int PhysicalMemorySimulator::find_suitable_segment_worst_fit(size_t required_size) {
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

void PhysicalMemorySimulator::split_memory_segment(int segment_index, size_t allocation_size) {
    MemorySegment& target_segment = heap_segments[segment_index];
    
    if (target_segment.segment_size > allocation_size) {
        size_t remaining_size = target_segment.segment_size - allocation_size;
        size_t new_segment_address = target_segment.base_address + allocation_size;
        
        target_segment.segment_size = allocation_size;
        
        heap_segments.insert(heap_segments.begin() + segment_index + 1,
                           MemorySegment(new_segment_address, remaining_size, true, -1));
    }
}

void PhysicalMemorySimulator::merge_adjacent_free_segments() {
    for (int i = 0; i < static_cast<int>(heap_segments.size()) - 1; ++i) {
        if (heap_segments[i].is_available && heap_segments[i + 1].is_available) {
            merge_with_next_segment(i);
            --i; // Check the same index again after merging
        }
    }
}

void PhysicalMemorySimulator::merge_with_next_segment(int current_index) {
    MemorySegment& current = heap_segments[current_index];
    MemorySegment& next = heap_segments[current_index + 1];
    
    if (current.base_address + current.segment_size == next.base_address) {
        current.segment_size += next.segment_size;
        heap_segments.erase(heap_segments.begin() + current_index + 1);
    }
}

void PhysicalMemorySimulator::display_memory_layout() const {
    std::cout << "\n--- Current Memory Layout ---\n";
    for (const auto& segment : heap_segments) {
        segment.display_info();
    }
    std::cout << "-----------------------------\n";
}

void PhysicalMemorySimulator::display_fragmentation_analysis() const {
    size_t total_free_space = 0;
    size_t total_allocated_space = 0;
    size_t largest_free_block = 0;
    
    for (const auto& segment : heap_segments) {
        if (segment.is_available) {
            total_free_space += segment.segment_size;
            largest_free_block = std::max(largest_free_block, segment.segment_size);
        } else {
            total_allocated_space += segment.segment_size;
        }
    }
    
    double memory_utilization = (total_memory_capacity > 0) ? 
        (100.0 * total_allocated_space) / total_memory_capacity : 0.0;
    
    double external_fragmentation = (total_free_space > 0) ?
        (100.0 * (total_free_space - largest_free_block)) / total_free_space : 0.0;
    
    std::cout << "\n--- Memory Analysis Report ---\n";
    std::cout << "Total memory capacity: " << total_memory_capacity << " bytes\n";
    std::cout << "Allocated memory: " << total_allocated_space << " bytes\n";
    std::cout << "Free memory: " << total_free_space << " bytes\n";
    std::cout << "Largest free block: " << largest_free_block << " bytes\n";
    std::cout << "Memory utilization: " << std::fixed << std::setprecision(2) 
              << memory_utilization << "%\n";
    std::cout << "External fragmentation: " << external_fragmentation << "%\n";
    std::cout << "Internal fragmentation: 0.00% (exact allocation)\n";
    std::cout << "-----------------------------\n";
    
    performance_stats.display_statistics();
}

void PhysicalMemorySimulator::reset_simulator() {
    heap_segments.clear();
    total_memory_capacity = 0;
    next_process_identifier = 1;
    performance_stats = MemoryStatistics();
    current_strategy = AllocationStrategy::FIRST_FIT;
    
    std::cout << "Memory simulator has been reset\n";
}

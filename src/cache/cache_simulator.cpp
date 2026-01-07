#include "cache_simulator.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

// CacheMetrics implementation
double CacheMetrics::get_hit_ratio() const {
    if (total_accesses == 0) return 0.0;
    return (100.0 * cache_hits) / total_accesses;
}

double CacheMetrics::get_miss_ratio() const {
    if (total_accesses == 0) return 0.0;
    return (100.0 * cache_misses) / total_accesses;
}

void CacheMetrics::display_metrics(const std::string& cache_name) const {
    std::cout << cache_name << " Performance:\n";
    std::cout << "  Total accesses: " << total_accesses << "\n";
    std::cout << "  Cache hits: " << cache_hits << "\n";
    std::cout << "  Cache misses: " << cache_misses << "\n";
    std::cout << "  Hit ratio: " << std::fixed << std::setprecision(2) 
              << get_hit_ratio() << "%\n";
    std::cout << "  Miss ratio: " << std::fixed << std::setprecision(2) 
              << get_miss_ratio() << "%\n";
}

// CacheLevel implementation
CacheLevel::CacheLevel(size_t cache_size, size_t block_size)
    : cache_size_bytes(cache_size), block_size_bytes(block_size) {
    
    if (block_size == 0) {
        throw std::invalid_argument("Block size cannot be zero");
    }
    
    number_of_blocks = cache_size / block_size;
    if (number_of_blocks == 0) {
        throw std::invalid_argument("Cache size must be at least one block size");
    }
    
    cache_blocks.resize(number_of_blocks);
    
    std::cout << "Cache initialized: " << cache_size << " bytes, " 
              << block_size << " bytes per block, " 
              << number_of_blocks << " total blocks\n";
}

bool CacheLevel::access_memory(size_t memory_address) {
    size_t block_index = calculate_block_index(memory_address);
    size_t tag = extract_tag(memory_address);
    
    // Check for cache hit
    if (cache_blocks[block_index].valid && cache_blocks[block_index].tag == tag) {
        performance_metrics.record_hit();
        return true; // Cache hit
    }
    
    // Cache miss - need to load from lower level
    performance_metrics.record_miss();
    
    // If the cache block is valid, we need to evict it (FIFO)
    if (cache_blocks[block_index].valid) {
        evict_oldest_block();
    }
    
    // Load new block
    cache_blocks[block_index].valid = true;
    cache_blocks[block_index].tag = tag;
    cache_blocks[block_index].data_address = memory_address;
    
    // Add to FIFO queue for replacement policy
    fifo_queue.push(block_index);
    
    return false; // Cache miss
}

size_t CacheLevel::calculate_block_index(size_t memory_address) const {
    return (memory_address / block_size_bytes) % number_of_blocks;
}

size_t CacheLevel::extract_tag(size_t memory_address) const {
    return memory_address / (block_size_bytes * number_of_blocks);
}

void CacheLevel::evict_oldest_block() {
    if (!fifo_queue.empty()) {
        size_t oldest_block = fifo_queue.front();
        fifo_queue.pop();
        cache_blocks[oldest_block].invalidate();
    }
}

void CacheLevel::flush_cache() {
    for (auto& block : cache_blocks) {
        block.invalidate();
    }
    
    // Clear FIFO queue
    while (!fifo_queue.empty()) {
        fifo_queue.pop();
    }
    
    std::cout << "Cache flushed\n";
}

void CacheLevel::display_cache_info() const {
    std::cout << "Cache Configuration:\n";
    std::cout << "  Size: " << cache_size_bytes << " bytes\n";
    std::cout << "  Block size: " << block_size_bytes << " bytes\n";
    std::cout << "  Number of blocks: " << number_of_blocks << "\n";
    
    int valid_blocks = 0;
    for (const auto& block : cache_blocks) {
        if (block.valid) valid_blocks++;
    }
    std::cout << "  Valid blocks: " << valid_blocks << "/" << number_of_blocks << "\n";
}

// MultiLevelCacheSimulator implementation
MultiLevelCacheSimulator::MultiLevelCacheSimulator() 
    : l1_cache(nullptr), l2_cache(nullptr), cache_hierarchy_initialized(false) {}

MultiLevelCacheSimulator::~MultiLevelCacheSimulator() {
    delete l1_cache;
    delete l2_cache;
}

void MultiLevelCacheSimulator::initialize_cache_hierarchy(size_t l1_size, size_t l1_block_size,
                                                        size_t l2_size, size_t l2_block_size) {
    // Clean up existing caches
    delete l1_cache;
    delete l2_cache;
    
    try {
        std::cout << "Initializing L1 Cache:\n";
        l1_cache = new CacheLevel(l1_size, l1_block_size);
        
        std::cout << "Initializing L2 Cache:\n";
        l2_cache = new CacheLevel(l2_size, l2_block_size);
        
        cache_hierarchy_initialized = true;
        std::cout << "Cache hierarchy successfully initialized\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error initializing cache hierarchy: " << e.what() << "\n";
        delete l1_cache;
        delete l2_cache;
        l1_cache = nullptr;
        l2_cache = nullptr;
        cache_hierarchy_initialized = false;
    }
}

bool MultiLevelCacheSimulator::simulate_memory_access(size_t memory_address) {
    if (!cache_hierarchy_initialized) {
        std::cout << "Error: Cache hierarchy not initialized\n";
        return false;
    }
    
    // Try L1 cache first
    if (l1_cache->access_memory(memory_address)) {
        // L1 hit - no need to access L2
        return true;
    }
    
    // L1 miss - try L2 cache
    if (l2_cache->access_memory(memory_address)) {
        // L2 hit - data found in L2
        return true;
    }
    
    // Both L1 and L2 miss - would access main memory
    return false;
}

void MultiLevelCacheSimulator::display_cache_statistics() const {
    if (!cache_hierarchy_initialized) {
        std::cout << "Cache hierarchy not initialized\n";
        return;
    }
    
    std::cout << "\n--- Cache Performance Statistics ---\n";
    l1_cache->get_metrics().display_metrics("L1 Cache");
    std::cout << "\n";
    l2_cache->get_metrics().display_metrics("L2 Cache");
    
    // Calculate overall statistics
    const auto& l1_metrics = l1_cache->get_metrics();
    const auto& l2_metrics = l2_cache->get_metrics();
    
    if (l1_metrics.total_accesses > 0) {
        double overall_hit_ratio = (100.0 * l1_metrics.cache_hits) / l1_metrics.total_accesses;
        if (l2_metrics.total_accesses > 0) {
            overall_hit_ratio += (100.0 * l2_metrics.cache_hits) / l1_metrics.total_accesses;
        }
        
        std::cout << "\nOverall Cache Performance:\n";
        std::cout << "  Combined hit ratio: " << std::fixed << std::setprecision(2) 
                  << overall_hit_ratio << "%\n";
    }
    std::cout << "-----------------------------------\n";
}

void MultiLevelCacheSimulator::flush_all_caches() {
    if (!cache_hierarchy_initialized) {
        std::cout << "Cache hierarchy not initialized\n";
        return;
    }
    
    std::cout << "Flushing all caches...\n";
    l1_cache->flush_cache();
    l2_cache->flush_cache();
}

void MultiLevelCacheSimulator::reset_cache_statistics() {
    if (cache_hierarchy_initialized) {
        // Reinitialize to reset statistics
        size_t l1_size = l1_cache->get_cache_size();
        size_t l1_block = l1_cache->get_block_size();
        size_t l2_size = l2_cache->get_cache_size();
        size_t l2_block = l2_cache->get_block_size();
        
        initialize_cache_hierarchy(l1_size, l1_block, l2_size, l2_block);
        std::cout << "Cache statistics reset\n";
    }
}

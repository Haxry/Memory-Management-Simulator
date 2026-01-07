#ifndef CACHE_SIMULATOR_HPP
#define CACHE_SIMULATOR_HPP

#include <vector>
#include <queue>
#include <iostream>

/**
 * @brief Represents a single cache line/block
 */
struct CacheBlock {
    bool valid;
    size_t tag;
    size_t data_address;
    
    CacheBlock() : valid(false), tag(0), data_address(0) {}
    void invalidate() { valid = false; tag = 0; data_address = 0; }
};

/**
 * @brief Cache performance metrics
 */
struct CacheMetrics {
    int total_accesses;
    int cache_hits;
    int cache_misses;
    
    CacheMetrics() : total_accesses(0), cache_hits(0), cache_misses(0) {}
    
    void record_hit() { total_accesses++; cache_hits++; }
    void record_miss() { total_accesses++; cache_misses++; }
    
    double get_hit_ratio() const;
    double get_miss_ratio() const;
    void display_metrics(const std::string& cache_name) const;
};

/**
 * @brief Single level cache simulator with FIFO replacement
 */
class CacheLevel {
private:
    std::vector<CacheBlock> cache_blocks;
    std::queue<size_t> fifo_queue;
    size_t cache_size_bytes;
    size_t block_size_bytes;
    size_t number_of_blocks;
    CacheMetrics performance_metrics;
    
    size_t calculate_block_index(size_t memory_address) const;
    size_t extract_tag(size_t memory_address) const;
    void evict_oldest_block();
    
public:
    CacheLevel(size_t cache_size, size_t block_size);
    ~CacheLevel() = default;
    
    bool access_memory(size_t memory_address);
    void flush_cache();
    void display_cache_info() const;
    
    // Getters
    const CacheMetrics& get_metrics() const { return performance_metrics; }
    size_t get_cache_size() const { return cache_size_bytes; }
    size_t get_block_size() const { return block_size_bytes; }
};

/**
 * @brief Multi-level cache hierarchy simulator
 */
class MultiLevelCacheSimulator {
private:
    CacheLevel* l1_cache;
    CacheLevel* l2_cache;
    bool cache_hierarchy_initialized;
    
public:
    MultiLevelCacheSimulator();
    ~MultiLevelCacheSimulator();
    
    void initialize_cache_hierarchy(size_t l1_size, size_t l1_block_size,
                                  size_t l2_size, size_t l2_block_size);
    
    bool simulate_memory_access(size_t memory_address);
    void display_cache_statistics() const;
    void flush_all_caches();
    void reset_cache_statistics();
    
    bool is_initialized() const { return cache_hierarchy_initialized; }
};

#endif // CACHE_SIMULATOR_HPP

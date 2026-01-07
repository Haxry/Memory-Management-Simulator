#include "memory_manager.hpp"
#include "command_processor.hpp"
#include "cache_simulator.hpp"
#include <iostream>
#include <memory>

/**
 * @brief Enhanced Memory Management and Cache Simulation System
 * @author Your Name
 * @version 2.0
 * 
 * This application simulates physical memory management with multiple
 * allocation strategies and includes a multi-level cache hierarchy
 * for comprehensive memory system analysis.
 */

class MemorySystemSimulator {
private:
    std::unique_ptr<PhysicalMemorySimulator> memory_manager;
    std::unique_ptr<MultiLevelCacheSimulator> cache_simulator;
    std::unique_ptr<CommandLineProcessor> command_interface;
    
public:
    MemorySystemSimulator() {
        memory_manager = std::make_unique<PhysicalMemorySimulator>();
        cache_simulator = std::make_unique<MultiLevelCacheSimulator>();
        command_interface = std::make_unique<CommandLineProcessor>(memory_manager.get());
    }
    
    void start_simulation() {
        display_system_banner();
        initialize_default_cache();
        command_interface->run_interactive_session();
    }
    
private:
    void display_system_banner() const {
        std::cout << "╔════════════════════════════════════════════════════════╗\n";
        std::cout << "║        Advanced Memory Management Simulator v2.0       ║\n";
        std::cout << "║                                                        ║\n";
        std::cout << "║  Features:                                             ║\n";
        std::cout << "║  • Dynamic Memory Allocation (First/Best/Worst Fit)   ║\n";
        std::cout << "║  • Multi-level Cache Simulation (L1/L2)               ║\n";
        std::cout << "║  • Comprehensive Performance Analytics                 ║\n";
        std::cout << "║  • Interactive Command Interface                       ║\n";
        std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
    }
    
    void initialize_default_cache() {
        // Initialize default cache hierarchy
        cache_simulator->initialize_cache_hierarchy(
            1024,   // L1: 1KB
            32,     // L1 block size: 32 bytes
            8192,   // L2: 8KB  
            64      // L2 block size: 64 bytes
        );
        
        std::cout << "Default cache hierarchy loaded.\n";
        std::cout << "Ready for memory management simulation.\n\n";
    }
};

int main() {
    try {
        MemorySystemSimulator simulator;
        simulator.start_simulation();
    }
    catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
    
    return 0;
}

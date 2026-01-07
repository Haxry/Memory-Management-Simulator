#include "command_processor.hpp"
#include <iostream>
#include <sstream>

CommandLineProcessor::CommandLineProcessor(PhysicalMemorySimulator* simulator)
    : memory_simulator(simulator), is_running(false) {}

void CommandLineProcessor::run_interactive_session() {
    display_welcome_message();
    is_running = true;
    
    std::string input_line;
    while (is_running) {
        std::cout << "memsim> ";
        std::getline(std::cin, input_line);
        
        if (!input_line.empty()) {
            process_single_command(input_line);
        }
    }
}

bool CommandLineProcessor::process_single_command(const std::string& command_line) {
    auto tokens = tokenize_command(command_line);
    
    if (tokens.empty()) return true;
    
    const std::string& command = tokens[0];
    
    try {
        if (command == "init" || command == "initialize") {
            handle_init_command(tokens);
        }
        else if (command == "strategy" || command == "set") {
            handle_strategy_command(tokens);
        }
        else if (command == "alloc" || command == "malloc") {
            handle_allocate_command(tokens);
        }
        else if (command == "free" || command == "dealloc") {
            handle_deallocate_command(tokens);
        }
        else if (command == "display" || command == "dump" || command == "show") {
            handle_display_command();
        }
        else if (command == "stats" || command == "statistics" || command == "analyze") {
            handle_statistics_command();
        }
        else if (command == "reset" || command == "clear") {
            handle_reset_command();
        }
        else if (command == "help" || command == "?") {
            display_help_message();
        }
        else if (command == "quit" || command == "exit" || command == "bye") {
            handle_exit_command();
        }
        else {
            std::cout << "Unknown command: '" << command << "'. Type 'help' for available commands.\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "Command execution error: " << e.what() << "\n";
    }
    
    return is_running;
}

std::vector<std::string> CommandLineProcessor::tokenize_command(const std::string& command_line) {
    std::vector<std::string> tokens;
    std::istringstream stream(command_line);
    std::string token;
    
    while (stream >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void CommandLineProcessor::handle_init_command(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cout << "Usage: init <memory_size>\n";
        std::cout << "Example: init 1024\n";
        return;
    }
    
    try {
        size_t memory_size = std::stoull(tokens[1]);
        if (memory_size == 0) {
            std::cout << "Error: Memory size must be greater than 0\n";
            return;
        }
        
        memory_simulator->initialize_memory_pool(memory_size);
    }
    catch (const std::invalid_argument&) {
        std::cout << "Error: Invalid memory size format\n";
    }
    catch (const std::out_of_range&) {
        std::cout << "Error: Memory size too large\n";
    }
}

void CommandLineProcessor::handle_strategy_command(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cout << "Usage: strategy <algorithm>\n";
        std::cout << "Available algorithms: first_fit, best_fit, worst_fit\n";
        return;
    }
    
    const std::string& algorithm = tokens[1];
    
    if (algorithm == "first_fit" || algorithm == "first" || algorithm == "ff") {
        memory_simulator->set_allocation_strategy(AllocationStrategy::FIRST_FIT);
    }
    else if (algorithm == "best_fit" || algorithm == "best" || algorithm == "bf") {
        memory_simulator->set_allocation_strategy(AllocationStrategy::BEST_FIT);
    }
    else if (algorithm == "worst_fit" || algorithm == "worst" || algorithm == "wf") {
        memory_simulator->set_allocation_strategy(AllocationStrategy::WORST_FIT);
    }
    else {
        std::cout << "Error: Unknown allocation algorithm '" << algorithm << "'\n";
        std::cout << "Available: first_fit, best_fit, worst_fit\n";
    }
}

void CommandLineProcessor::handle_allocate_command(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cout << "Usage: alloc <size>\n";
        std::cout << "Example: alloc 256\n";
        return;
    }
    
    try {
        size_t allocation_size = std::stoull(tokens[1]);
        memory_simulator->allocate_memory_block(allocation_size);
    }
    catch (const std::invalid_argument&) {
        std::cout << "Error: Invalid allocation size format\n";
    }
    catch (const std::out_of_range&) {
        std::cout << "Error: Allocation size too large\n";
    }
}

void CommandLineProcessor::handle_deallocate_command(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cout << "Usage: free <process_id>\n";
        std::cout << "Example: free 3\n";
        return;
    }
    
    try {
        int process_id = std::stoi(tokens[1]);
        memory_simulator->deallocate_memory_block(process_id);
    }
    catch (const std::invalid_argument&) {
        std::cout << "Error: Invalid process ID format\n";
    }
    catch (const std::out_of_range&) {
        std::cout << "Error: Process ID out of range\n";
    }
}

void CommandLineProcessor::handle_display_command() {
    memory_simulator->display_memory_layout();
}

void CommandLineProcessor::handle_statistics_command() {
    memory_simulator->display_fragmentation_analysis();
}

void CommandLineProcessor::handle_reset_command() {
    memory_simulator->reset_simulator();
}

void CommandLineProcessor::handle_exit_command() {
    std::cout << "Goodbye!\n";
    is_running = false;
}

void CommandLineProcessor::display_welcome_message() const {
    std::cout << "===========================================\n";
    std::cout << "  Physical Memory Management Simulator\n";
    std::cout << "===========================================\n";
    std::cout << "Type 'help' to see available commands\n";
    std::cout << "Type 'exit' to quit the simulator\n\n";
}

void CommandLineProcessor::display_help_message() const {
    std::cout << "\n--- Available Commands ---\n";
    std::cout << "init <size>           - Initialize memory pool with specified size\n";
    std::cout << "strategy <algorithm>  - Set allocation strategy (first_fit/best_fit/worst_fit)\n";
    std::cout << "alloc <size>          - Allocate memory block of specified size\n";
    std::cout << "free <pid>            - Deallocate memory block with process ID\n";
    std::cout << "display               - Show current memory layout\n";
    std::cout << "stats                 - Display memory statistics and analysis\n";
    std::cout << "reset                 - Reset the entire simulator\n";
    std::cout << "help                  - Show this help message\n";
    std::cout << "exit                  - Quit the simulator\n";
    std::cout << "-------------------------\n";
}

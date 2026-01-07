#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include "memory_manager.hpp"
#include <string>
#include <sstream>

/**
 * @brief Command line interface processor for the memory simulator
 */
class CommandLineProcessor {
private:
    PhysicalMemorySimulator* memory_simulator;
    bool is_running;
    
    // Command parsing helpers
    std::vector<std::string> tokenize_command(const std::string& command_line);
    void display_help_message() const;
    void display_welcome_message() const;
    
    // Individual command handlers
    void handle_init_command(const std::vector<std::string>& tokens);
    void handle_strategy_command(const std::vector<std::string>& tokens);
    void handle_allocate_command(const std::vector<std::string>& tokens);
    void handle_deallocate_command(const std::vector<std::string>& tokens);
    void handle_display_command();
    void handle_statistics_command();
    void handle_reset_command();
    void handle_exit_command();
    
public:
    explicit CommandLineProcessor(PhysicalMemorySimulator* simulator);
    ~CommandLineProcessor() = default;
    
    void run_interactive_session();
    bool process_single_command(const std::string& command_line);
    void stop_session() { is_running = false; }
};

#endif // COMMAND_PROCESSOR_HPP

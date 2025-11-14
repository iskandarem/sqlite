#pragma once
#include <string>
#include "Statement.hpp"
#include "Enums.hpp"

class InputBuffer
{
private:
    std::string buffer; // Pointer to buffer holding input string
public:

    const std::string& get_buffer() const;

    bool set_buffer(const std::string& input);

    bool read_input();

    MetaCommandResult do_meta_command();

    PrepareResult prepare_statement(Statement& statement);
};


std::string trim_whitespace(const std::string& input);

#pragma once
#include <string>
#include "Statement.hpp"
#include "Enums.hpp"

class InputBuffer
{
private:
    std::string* buffer; // Pointer to buffer holding input string
public:
    InputBuffer();

    std::string* get_buffer();

    bool set_buffer(std::string input);

    bool read_input();

    MetaCommandResult do_meta_command(Table* table);

    PrepareResult prepare_statement(Statement* statement);

    ~InputBuffer();   
};


void trim_whitespace(std::string& str);

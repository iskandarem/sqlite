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

    void set_buffer(std::string input);

    void read_input();

    MetaCommandResult do_meta_command();

    PrepareResult prepare_statement(Statement* statement);

    ~InputBuffer();   
};


void trim_whitespace(std::string& str);

#include<iostream>
#include "InputBuffer.hpp"
#include "Statement.hpp"

void print_prompt()
{
    std::cout << "db> ";
}

int main(int argc, char* argv[])
{
    InputBuffer* input_buffer = new InputBuffer();
    Table* table = new Table();
    while (true)
    {
        print_prompt();
        input_buffer->read_input();

        if ((*input_buffer->get_buffer())[0] == '.')
        {
            switch (input_buffer->do_meta_command())
            {
            case META_COMMAND_SUCCESS:
                continue;
            case META_COMMAND_UNRECOGNIZED_COMMAND:
                std::cout << "Unrecognized command '" << *input_buffer->get_buffer() << "'" << std::endl;
                continue;
            }
        }
        Statement statement;
        switch (input_buffer->prepare_statement(&statement))
        {
        case PREPARE_SUCCESS:
            break;
        case PREPARE_SYNTAX_ERROR:
            std::cout << "Syntax error. Could not parse statement." << std::endl;
            continue;
        case PREPARE_UNRECOGNIZED_STATEMENT:
            std::cout << "Unrecognized keyword at start of '" << *input_buffer->get_buffer() << "'." << std::endl;
            continue;
        }
        switch (statement.execute_statement(table))
        {
        case EXECUTE_SUCCESS:
            std::cout << "Executed." << std::endl; 
            break;
        case EXECUTE_TABLE_FULL:
            std::cout << "Error: Table full." << std::endl;
            break;
        }
    }
}
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
        case PREPARE_UNRECOGNIZED_STATEMENT:
            std::cout << "Unrecognized keyword at start of '" << *input_buffer->get_buffer() << "'." << std::endl;
            continue;
        }
        statement.execute_statement();
        std::cout << "Executed." << std::endl;
    }
}
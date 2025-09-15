#include<iostream>
#include "InputBuffer.h"

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

        if((*input_buffer->get_buffer()).compare(".exit") == 0)
        {
            delete input_buffer; //close input buffer 
            exit(EXIT_SUCCESS); //exit with success
        }
        else
        {
            std::cout << "Unrecognized command '" << *input_buffer->get_buffer() << "'." << std::endl;
        }
    }
    
}
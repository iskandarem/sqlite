#include "InputBuffer.hpp"
#include <iostream>
#include <algorithm>

InputBuffer::InputBuffer()
    :buffer(new std::string()) {}

std::string* InputBuffer::get_buffer()
{
    return this->buffer;
}

void InputBuffer::read_input()
{
    std::getline(std::cin, *buffer);
    if(buffer->empty())
    {
        std::cout << "Error reading input" << std::endl;
        exit(EXIT_FAILURE);
    }
    trim_whitespace(*buffer);
}

InputBuffer::~InputBuffer()
{
    delete buffer; // Releases buffer memory
}


void trim_whitespace(std::string& str)
{
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), ::isspace));
    str.erase(std::find_if_not(str.rbegin(), str.rend(), ::isspace).base(), str.end());
}
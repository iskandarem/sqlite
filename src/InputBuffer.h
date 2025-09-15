#include<cstdlib>
#include<cstddef>
#include <string>
#include<iostream>
#include<memory>

class InputBuffer
{
private:
    std::string* buffer;
    ssize_t input_length;
public:
    InputBuffer()
        :buffer(new std::string()), input_length(0){}

    std::string* get_buffer();

    ssize_t get_input_length();

    void read_input();

    ~InputBuffer()
    {
        delete buffer; // deleting string 
    }
        
};

inline std::string* InputBuffer::get_buffer()
{
    return this->buffer;
}

inline ssize_t InputBuffer::get_input_length()
{
    return this->input_length;
}

inline void InputBuffer::read_input()
{
    std::getline(std::cin, *buffer);
    if(buffer->empty())
    {
        std::cout << "Error reading input" << std::endl;
        exit(EXIT_FAILURE);
    }
}



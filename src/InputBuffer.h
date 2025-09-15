#include<cstdlib>
#include<cstddef>
#include <string>
#include<iostream>
#include<memory>


class InputBuffer
{
private:
    std::string* buffer; // Pointer to buffer holding input string
public:
    InputBuffer()
        :buffer(new std::string()) {}

    std::string* get_buffer();


    void read_input();

    ~InputBuffer()
    {
        delete buffer; // Releases buffer memory 
    }
        
};

inline std::string* InputBuffer::get_buffer()
{
    return this->buffer;
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



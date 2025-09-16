#pragma once
#include <string>
class InputBuffer
{
private:
    std::string* buffer; // Pointer to buffer holding input string
public:
    InputBuffer();

    std::string* get_buffer();


    void read_input();

    ~InputBuffer();   
};


void trim_whitespace(std::string& str);

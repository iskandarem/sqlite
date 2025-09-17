#include "InputBuffer.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

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

MetaCommandResult InputBuffer::do_meta_command()
{
    if (*buffer == ".exit")
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult InputBuffer::prepare_statement(Statement *statement)
{
    // std::string temp = buffer->substr(0, 6);
    std::istringstream iss(*this->buffer);
    std::string keyword;
    iss >> keyword;
    if (keyword == "insert")
    {
        statement->type = STATEMENT_INSERT;
        if(!(iss >> (statement->row_to_insert.id) >> statement->row_to_insert.username >> statement->row_to_insert.email))
            return PREPARE_SYNTAX_ERROR;
        return PREPARE_SUCCESS;
    }
    if (*buffer == "select")
    {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
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
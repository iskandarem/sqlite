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

bool InputBuffer::set_buffer(std::string input)
{
    trim_whitespace(input);
    if(input.empty())
    {
        std::cout << "Cannot assign '" << input << "' to buffer." << std::endl;
        // exit(EXIT_FAILURE);
        return false;
    }
    *buffer = input;
    return true;
}

bool InputBuffer::read_input()
{
    std::getline(std::cin, *buffer);
    if(buffer->empty())
    {
        std::cout << "Error reading input" << std::endl;
        // exit(EXIT_FAILURE);
        return false;
    }
    trim_whitespace(*buffer);
    return set_buffer(*buffer);
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
        int id;
        std::string username;
        std::string email;
        if(!(iss >> id >> username >> email))
            return PREPARE_SYNTAX_ERROR;
        
        statement->row_to_insert.set_id(id);
        if(!(statement->row_to_insert.set_username(username) && statement->row_to_insert.set_email(email)))
        {
            return PREPARE_STRING_TOO_LONG;
        }
        
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
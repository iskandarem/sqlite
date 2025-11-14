#include "InputBuffer.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>


const std::string& InputBuffer::get_buffer() const 
{
    return this->buffer;
}

bool InputBuffer::set_buffer(const std::string& input)
{
    
    if(trim_whitespace(input).empty())
    {
        std::cout << "Cannot assign '" << input << "' to buffer." << std::endl;
        // exit(EXIT_FAILURE);
        return false;
    }
    buffer = input;
    return true;
}

bool InputBuffer::read_input()
{
    std::getline(std::cin, buffer);
    if(buffer.empty())
    {
        std::cout << "Error reading input\n";
        // exit(EXIT_FAILURE);
        return false;
    }
    buffer = trim_whitespace(buffer);
    return set_buffer(buffer);
}

MetaCommandResult InputBuffer::do_meta_command()
{
    if (buffer == ".exit")
    {
        return META_COMMAND_EXIT_SUCCESS;
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
    
}

PrepareResult InputBuffer::prepare_statement(Statement& statement)
{
    std::istringstream iss(buffer);
    std::string keyword;
    iss >> keyword;
    if (keyword == "insert")
    {
        statement.type = STATEMENT_INSERT;
        int id;
        std::string username;
        std::string email;
        if(!(iss >> id >> username >> email)) return PREPARE_SYNTAX_ERROR;
        
        if(!statement.row_to_insert.set_id(id)) return PREPARE_NEGATIVE_ID;

        if(!(statement.row_to_insert.set_username(username) && statement.row_to_insert.set_email(email))) return PREPARE_STRING_TOO_LONG;
        
        return PREPARE_SUCCESS;
    }
    if (buffer == "select")
    {
        statement.type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

std::string trim_whitespace(const std::string& input)
{
    std::string str = input;
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), ::isspace));
    str.erase(std::find_if_not(str.rbegin(), str.rend(), ::isspace).base(), str.end());
    return str;
}
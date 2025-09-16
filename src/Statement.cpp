#include "Statement.hpp"
#include <iostream>

void Statement::execute_statement()
{
    switch (this->type)
    {
    case STATEMENT_INSERT:
        std::cout << "This is where we would do an insert." << std::endl;
        break;
    case STATEMENT_SELECT:
        std::cout << "This is where we would do a select." << std::endl;
        break;
    }
}
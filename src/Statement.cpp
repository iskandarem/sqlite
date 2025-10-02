#include "Statement.hpp"
#include <iostream>

ExecuteResult Statement::execute_insert(Table *table)
{
    if(table->num_rows >= TABLE_MAX_ROWS) return EXECUTE_TABLE_FULL;

    Cursor* cursor = table->end();
    
    this->row_to_insert.serialize(cursor->get_value());
    ++table->num_rows;
    return EXECUTE_SUCCESS;    
}

ExecuteResult Statement::execute_select(Table *table)
{
    Row row;

    Cursor* cursor = table->begin();
    
    while(!cursor->end_of_table)
    {
        row.deserialize(cursor->get_value());
        std::cout << row << std::endl;
        cursor->advance();
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult Statement::execute_statement(Table *table)
{
    switch (this->type)
    {
        case STATEMENT_INSERT:
            return this->execute_insert(table);
            break;
        case STATEMENT_SELECT:
            return this->execute_select(table);
            break;
    }
    return EXECUTE_TABLE_FULL;
}

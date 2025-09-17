#include "Statement.hpp"
#include <iostream>

ExecuteResult Statement::execute_insert(Table *table)
{
    if(table->num_rows >= TABLE_MAX_ROWS) return EXECUTE_TABLE_FULL;
    this->row_to_insert.serialize((*table)[table->num_rows]);
    ++table->num_rows;
    return EXECUTE_SUCCESS;    
}

ExecuteResult Statement::execute_select(Table *table)
{
    Row row;
    for (uint32_t i = 0; i < table->num_rows; ++i)
    {
        row.deserialize((*table)[i]);
        std::cout << row << std::endl;
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

#pragma once
#include "Enums.hpp"
#include "Row.hpp"
#include "Table.hpp"


struct Statement
{
    StatementType type;
    Row row_to_insert; // will be used only in insert statement.
    ExecuteResult execute_insert(Table* table);
    ExecuteResult execute_select(Table* table);
    ExecuteResult execute_statement(Table* table);
};


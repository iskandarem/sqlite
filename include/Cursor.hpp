#pragma once
#include "Table.hpp"

class Table;

struct Cursor
{
    Table* table;
    uint32_t row_num;
    bool end_of_table; // Indicates a position one past the last element
    char* get_value();
    void advance();
};

#pragma once
#include <cstdint>
#include "Row.hpp"
#include "Pager.hpp"
#include "Cursor.hpp"


const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE*MAX_TABLE_PAGES;

class Cursor;

class Table
{
public:
    uint32_t num_rows;
    Pager* pager;
    Cursor* begin();
    Cursor* end();
    Table(std::string filename);
    ~Table();
};

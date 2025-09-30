#pragma once
#include <cstdint>
#include "Row.hpp"
#include "Pager.hpp"


const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE*MAX_TABLE_PAGES;

class Table
{
public:
    uint32_t num_rows;
    Pager* pager;
    std::byte* operator [](uint32_t row_num);
    Table(std::string filename);
    ~Table();
};

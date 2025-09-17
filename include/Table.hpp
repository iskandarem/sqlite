#pragma once
#include <cstdint>
#include "Row.hpp"

const uint32_t PAGE_SIZE = 4096;
#define MAX_TABLE_PAGES  100
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE*MAX_TABLE_PAGES;

class Table
{
public:
    uint32_t num_rows;
    std::byte* pages[MAX_TABLE_PAGES];   
    std::byte* operator [](uint32_t row_num);
    Table();
    ~Table();
    
};

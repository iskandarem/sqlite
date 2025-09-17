#include "Table.hpp"
#include <memory>

std::byte* Table::operator[](uint32_t row_num)
{

    uint32_t page_num = row_num / ROWS_PER_PAGE;
    if (!this->pages[page_num]) {
        this->pages[page_num] = static_cast<std::byte*>(malloc(PAGE_SIZE));
    }
    std::byte* page = this->pages[page_num];
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

Table::Table()
{
    num_rows = 0;
    for (uint32_t i = 0; i < MAX_TABLE_PAGES; ++i)
    {
        pages[i] = nullptr;
    }
}

Table::~Table()
{
    for (uint32_t i = 0; i < MAX_TABLE_PAGES; ++i)
    {
        delete[] pages[i];
    }
}

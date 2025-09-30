#include "Table.hpp"
#include <memory>

std::byte* Table::operator[](uint32_t row_num)
{

    uint32_t page_num = row_num / ROWS_PER_PAGE;
    std::byte* page = pager->get_page(page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

Table::Table(std::string filename)
    :pager(new Pager(filename))
{
    num_rows = 0;
}

Table::~Table()
{
    uint32_t num_of_full_pages = num_rows / ROWS_PER_PAGE;
    if(num_rows % ROWS_PER_PAGE > 0) ++num_of_full_pages;
    for(uint32_t i=0; i<num_of_full_pages; ++i)
    {
        if(!pager->get_page_raw(i)) continue;
        pager->pager_flush(i);
        delete pager->get_page_raw(i);
        pager->set_page(nullptr, i);
    }
    delete pager;
    
}

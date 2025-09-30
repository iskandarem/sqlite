#include "Table.hpp"
#include <memory>
#include <cstring>

char* Table::operator[](uint32_t row_num)
{

    uint32_t page_num = row_num / ROWS_PER_PAGE;
    char* page = pager->get_page(page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

Table::Table(std::string filename)
    :pager(new Pager(filename))
{
    // Initialize num_rows by counting rows with non-zero id on disk
    num_rows = 0;
    uint32_t file_length = pager->get_file_length();
    if (file_length > 0)
    {
        uint32_t num_pages = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE) ++num_pages;
        for (uint32_t p = 0; p < num_pages; ++p)
        {
            char* page = pager->get_page(p);
            if (!page) continue;
            for (uint32_t r = 0; r < ROWS_PER_PAGE; ++r)
            {
                uint32_t row_index = p * ROWS_PER_PAGE + r;
                char* row_ptr = page + r * ROW_SIZE;
                uint32_t id = 0;
                memcpy(&id, row_ptr + ID_OFFSET, ID_SIZE);
                if (id == 0) return;  
                ++num_rows;
            }
        }
    }
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

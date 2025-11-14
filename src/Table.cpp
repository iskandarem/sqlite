#include "Table.hpp"
#include <memory>
#include <cstring>


Cursor *Table::begin() 
{
    Cursor* cursor = new Cursor();
    cursor->table = this;
    cursor->row_num = 0;
    cursor->end_of_table = (this->num_rows == 0);
    return cursor;
}

Cursor *Table::end()
{
    Cursor* cursor = new Cursor();
    cursor->table = this;
    cursor->row_num = this->num_rows;
    cursor->end_of_table = true;
    return cursor;
}

Table::Table(std::string filename)
    :pager(new Pager(filename))
{
    // Initialize num_rows by counting rows with non-zero id on disk
    num_rows = 0;
    size_t file_length = pager->get_file_length();
    if (file_length > 0)
    {
        size_t num_pages = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE) ++num_pages;
        for (size_t p = 0; p < num_pages; ++p)
        {
            char* page = pager->get_page(p);
            if (!page) continue;
            for (size_t r = 0; r < ROWS_PER_PAGE; ++r)
            {
                size_t row_index = p * ROWS_PER_PAGE + r;
                char* row_ptr = page + r * ROW_SIZE;
                size_t id = 0;
                memcpy(&id, row_ptr + ID_OFFSET, ID_SIZE);
                if (id == 0) return;  
                ++num_rows;
            }
        }
    }
}

Table::~Table()
{
    std::cout << "Closing table. Flushing pages to disk..." << std::endl;
    size_t num_of_full_pages = num_rows / ROWS_PER_PAGE;
    if(num_rows % ROWS_PER_PAGE > 0) ++num_of_full_pages;
    for(size_t i=0; i<num_of_full_pages; ++i)
    {
        if(!pager->get_page_raw(i)) continue;
        pager->pager_flush(i);
        delete pager->get_page_raw(i);
        pager->set_page(nullptr, i);
    }
    delete pager;
}

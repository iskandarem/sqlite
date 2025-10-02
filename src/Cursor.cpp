#include "Cursor.hpp"

char *Cursor::get_value()
{
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    char* page = table->pager->get_page(page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

void Cursor::advance()
{
    ++row_num;
    if(row_num >= table->num_rows) 
    {
        end_of_table = true;
    }
}

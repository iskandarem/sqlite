#pragma once
#include <cstdint>
#include <fstream>


#define MAX_TABLE_PAGES  100
const uint32_t PAGE_SIZE = 4096;

class Pager
{
private:
    uint32_t file_length;
    char* pages[MAX_TABLE_PAGES];
    std::fstream file;
public:
    Pager(std::string filename);
    uint32_t get_file_length();
    char* get_page_raw(uint32_t page_num);
    void set_page(char* page, uint32_t page_num);
    char* get_page(uint32_t page_num);
    void pager_flush(uint32_t page_num);
    ~Pager();
};

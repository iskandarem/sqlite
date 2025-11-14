#pragma once
#include <cstdint>
#include <fstream>


constexpr size_t MAX_TABLE_PAGES = 100;
const size_t PAGE_SIZE = 4096;

class Pager
{
private:
    size_t file_length;
    char* pages[MAX_TABLE_PAGES];
    std::fstream file;
public:
    Pager(std::string filename);
    size_t get_file_length();
    char* get_page_raw(size_t page_num);
    void set_page(char* page, size_t page_num);
    char* get_page(size_t page_num);
    void pager_flush(size_t page_num);
    ~Pager();
};

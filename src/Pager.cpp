#include "Pager.hpp"
#include <iostream>

Pager::Pager(const std::string filename)
    :file(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc)
{
    if(!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }
    file.exceptions(std::ios::badbit | std::ios::failbit);
    try
    {
        file.seekg(0, std::fstream::end);
        file_length = file.tellg();
        file.seekg(0, std::ios::beg);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    for(int i=0; i<MAX_TABLE_PAGES; ++i)
    {
        pages[i] = nullptr;
    }
}

uint32_t Pager::get_file_length()
{
    return file_length;
}

std::byte *Pager::get_page_raw(uint32_t page_num)
{
    return pages[page_num];
}

void Pager::set_page(std::byte *page, uint32_t page_num)
{
    pages[page_num] = page;
}

std::byte *Pager::get_page(uint32_t page_num)
{
    if(page_num>MAX_TABLE_PAGES)
    {
        throw std::runtime_error("Tried to fetch page number out of bounds. " + std::to_string(page_num) + '>' + std::to_string(MAX_TABLE_PAGES) + ".\nError at " + __FILE__ + ":" + std::to_string(__LINE__));
    }
    if(!pages[page_num])
    {
        std::byte* page = new std::byte[PAGE_SIZE];
        uint32_t num_pages = file_length/PAGE_SIZE;
        if(file_length%PAGE_SIZE) ++num_pages;
        if(page_num <= num_pages)
        {
            try
            {
                file.seekg(page_num * PAGE_SIZE, std::ios::beg);
                file.read(reinterpret_cast<char*>(page), PAGE_SIZE);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        pages[page_num] = page;
    }
    return pages[page_num];
}

void Pager::pager_flush(uint32_t page_num)
{
    if(!pages[page_num]) 
    {
        throw std::runtime_error("Tried to flush null page.\n");
    }
    try
    {
        file.seekp(page_num * page_num, std::ios::beg);
        file.write(reinterpret_cast<char*>(pages[page_num]), PAGE_SIZE);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

Pager::~Pager()
{
    for(uint32_t i=0; i<MAX_TABLE_PAGES; ++i)
    {
        if(pages[i]) delete pages[i];
        pages[i] = nullptr;
    }
}

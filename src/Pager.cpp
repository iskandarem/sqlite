#include "Pager.hpp"
#include <iostream>

Pager::Pager(const std::string filename)
    :file(filename, std::ios::in | std::ios::out | std::ios::binary)
{
    // If the file doesn't exist yet, opening with in|out will fail.
    // Try to create the file and reopen it so we can read/write.
    if(!file.is_open())
    {
        std::fstream create_file(filename, std::ios::out | std::ios::binary);
        if(!create_file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filename);
        }
        create_file.close();
        // Reopen for read/write
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if(!file.is_open())
        {
            throw std::runtime_error("Could not open file after creating: " + filename);
        }
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

char *Pager::get_page_raw(uint32_t page_num)
{
    return pages[page_num];
}

void Pager::set_page(char *page, uint32_t page_num)
{
    pages[page_num] = page;
}

char *Pager::get_page(uint32_t page_num)
{
    if(page_num>=MAX_TABLE_PAGES)
    {
        throw std::runtime_error("Tried to fetch page number out of bounds. " + std::to_string(page_num) + '>' + std::to_string(MAX_TABLE_PAGES) + ".\nError at " + __FILE__ + ":" + std::to_string(__LINE__));
    }
    if(!pages[page_num])
    {
        char* page = new char[PAGE_SIZE]();
        uint32_t num_pages = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE) ++num_pages;
        // Only attempt to read from file if the requested page already exists on disk
        if (page_num < num_pages)
        {
            try
            {
                file.seekg(page_num * PAGE_SIZE, std::ios::beg);
                file.read(page, PAGE_SIZE);
            }
            catch(const std::exception& e)
            {
                // Read failed;
                std::cerr << e.what() << '\n';
                file.clear();
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
        file.seekp(page_num * PAGE_SIZE, std::ios::beg);
        file.write(pages[page_num], PAGE_SIZE);
        file.flush();
        // Update file_length if we extended the file
        try {
            file.seekp(0, std::ios::end);
            file_length = file.tellp();
        } catch (...) {}
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
        if(pages[i]) delete[] pages[i];
        pages[i] = nullptr;
    }
    file.close();
}

#pragma once
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <array>
#include <string>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255



class Row
{
private:
    size_t id;
    std::array<char, COLUMN_USERNAME_SIZE+1> username;
    std::array<char, COLUMN_EMAIL_SIZE+1> email;
public:
    Row() = default;
    bool set_id(int _id);
    bool set_username(std::string _username);
    bool set_email(std::string _email);
    int get_id();
    char* get_username();
    char* get_email();
    void serialize(char* destination);
    void deserialize(char* destination);
    static const size_t size_of_id();
    static const size_t size_of_username();
    static const size_t size_of_email();
    friend std::ostream& operator<<(std::ostream& os, const Row& row);
    friend std::istream& operator>>(std::istream& is, Row& row);
    ~Row() = default;
};


const size_t ID_SIZE = Row::size_of_id();
const size_t USERNAME_SIZE = Row::size_of_username();
const size_t EMAIL_SIZE = Row::size_of_email();
const size_t ID_OFFSET = 0;
const size_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const size_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const size_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
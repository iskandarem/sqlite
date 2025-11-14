#include "Row.hpp"
#include <cstring>
#include <string>

bool Row::set_id(int _id)
{
    if(_id<=0) return false;
    this->id = _id;
    return true;
}

bool Row::set_username(std::string _username)
{
    if(_username.length()>COLUMN_USERNAME_SIZE) return false;
    for(int i=0; i<_username.length(); ++i)
        this->username[i] = _username[i];
    this->username[_username.length()] = '\0';
    return true;
}

bool Row::set_email(std::string _email)
{
    if(_email.length()>COLUMN_EMAIL_SIZE)
        return false;
    
    for(int i=0; i<_email.length(); ++i)
    {
        email[i] = _email[i];
    }
    this->email[_email.length()] = '\0';
    return true;
}

void Row::serialize(char *destination)
{
    memcpy(destination+ID_OFFSET, &(this->id), ID_SIZE);
    strncpy(destination + USERNAME_OFFSET, this->username.data(), USERNAME_SIZE);
    strncpy(destination + EMAIL_OFFSET, this->email.data(), EMAIL_SIZE);
}

void Row::deserialize(char* source)
{
    memcpy(&(this->id), source + ID_OFFSET, ID_SIZE);
    memcpy(this->username.data(), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(this->email.data(), source + EMAIL_OFFSET, EMAIL_SIZE);
}

const size_t Row::size_of_id()
{
    return sizeof(Row::id);
}

const size_t Row::size_of_username()
{
    return sizeof(Row::username);
}

const size_t Row::size_of_email()
{
    return sizeof(Row::email);
}

int Row::get_id()
{
    return id;
}

char* Row::get_username()
{
    return this->username.data();
}

char* Row::get_email()
{
    return this->email.data();
}

std::ostream& operator<<(std::ostream& os, const Row& row)
{
    os << row.id << " " << row.username.data() << " " << row.email.data();
    return os;
}

std::istream& operator>>(std::istream& is, Row& row)
{
    std::string username_str, email_str;
    is >> row.id >> username_str >> email_str;
    row.set_username(username_str);
    row.set_email(email_str);
    return is;
}
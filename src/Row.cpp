#include "Row.hpp"
#include <cstring>

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
    return true;
}

void Row::serialize(std::byte *destination)
{
    memcpy(destination+ID_OFFSET, &(this->id), ID_SIZE);
    memcpy(destination+USERNAME_OFFSET, &(this->username), USERNAME_SIZE);
    memcpy(destination+EMAIL_OFFSET, &(this->email), EMAIL_SIZE);
}

void Row::deserialize(std::byte* source)
{
    memcpy(&(this->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(this->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(this->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

const uint32_t Row::size_of_id()
{
    return sizeof(Row::id);
}

const uint32_t Row::size_of_username()
{
    return sizeof(Row::username);
}

const uint32_t Row::size_of_email()
{
    return sizeof(Row::email);
}

std::ostream& operator<<(std::ostream& os, const Row& row)
{
    os << row.id << " " << row.username << " " << row.email;
    return os;
}

std::istream& operator>>(std::istream& is, Row& row)
{
    is >> row.id >> row.username >> row.email ;
    return is;
}
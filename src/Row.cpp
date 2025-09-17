#include "Row.hpp"
#include <cstring>

void Row::serialize(std::byte* destination)
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
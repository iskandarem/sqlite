#pragma once
#include "Enums.hpp"



typedef struct
{
    StatementType type;
    void execute_statement();
}Statement;
#pragma once

typedef enum
{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum 
{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

typedef enum
{
    STATEMENT_INSERT,
    STATEMENT_SELECT    
}StatementType;




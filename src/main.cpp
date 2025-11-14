#include<iostream>
#include "InputBuffer.hpp"
#include "Statement.hpp"

void print_prompt()
{
    std::cout << "db> ";
}

void test_full_table_exit()
{
    std::string filename = "test_db_for_full_table_exit.db";
    std::ofstream f(filename, std::ios::trunc);
    f.close();
    Table table(filename);
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("insert 1 user1 user@gmail.com");
    input_buffer.prepare_statement(statement);
    size_t count = 1;
    while(statement.execute_insert(table) != EXECUTE_TABLE_FULL)
    {
        ++count;
        std::string s = std::to_string(count);
        input_buffer.set_buffer("insert " + s + " person" + s + " email.person" + s + "@gmail.com");
        input_buffer.prepare_statement(statement);
    }
    std::cout << "Inserted " << count-1 << " rows before table was full." << std::endl;
}

int main(int argc, char* argv[])
{
    test_full_table_exit();
    return 0;

    // std::cout << "Maximum number of rows: " << TABLE_MAX_ROWS << std::endl;
    // if(argc < 2)
    // {
    //     throw std::runtime_error("Must supply a database filename.\n");
    // }
    // std::string filename;
    // filename = argv[1];
    
    // InputBuffer input_buffer;
    // Table table(filename);
    // while (true)
    // {
    //     print_prompt();
    //     if(!input_buffer.read_input()) continue;

    //     if ((input_buffer.get_buffer())[0] == '.')
    //     {
    //         switch (input_buffer.do_meta_command())
    //         {
    //             case META_COMMAND_EXIT_SUCCESS:
    //                 exit(EXIT_SUCCESS);
    //                 break;    
    //             case META_COMMAND_SUCCESS:
    //                 continue;
    //             case META_COMMAND_UNRECOGNIZED_COMMAND:
    //                 std::cout << "Unrecognized command '" << input_buffer.get_buffer() << "'" << std::endl;
    //                 continue;
    //         }
    //     }
    //     Statement statement;
    //     switch (input_buffer.prepare_statement(statement))
    //     {
    //     case PREPARE_SUCCESS:
    //         break;
    //     case PREPARE_NEGATIVE_ID:
    //         std::cout << "id cannot be 0 or negative.\n";
    //         continue;
    //     case PREPARE_STRING_TOO_LONG:
    //         std::cout << "Username or email is/are too long.\n Valid parameters:\n\tusername: " << COLUMN_USERNAME_SIZE << std::endl << "\temail: " << COLUMN_EMAIL_SIZE << std::endl;
    //         continue;
    //     case PREPARE_SYNTAX_ERROR:
    //         std::cout << "Syntax error. Could not parse statement." << std::endl;
    //         continue;
    //     case PREPARE_UNRECOGNIZED_STATEMENT:
    //         std::cout << "Unrecognized keyword at start of '" << input_buffer.get_buffer() << "'." << std::endl;
    //         continue;
    //     }
    //     switch (statement.execute_statement(table))
    //     {
    //     case EXECUTE_SUCCESS:
    //         std::cout << "Executed." << std::endl; 
    //         break;
    //     case EXECUTE_TABLE_FULL:
    //         std::cout << "Error: Table full." << std::endl;
    //         break;
    //     }
    // }
}
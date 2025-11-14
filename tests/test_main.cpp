#include <gtest/gtest.h>
#include <InputBuffer.hpp>

namespace
{
    std::string filename = "test_db_for_tests.db";
    void cleanup()
    {
        std::fstream f(filename, std::ios::out | std::ios::trunc);
        f.close();
    }
}

TEST(DoMetaCommandTest, BasicAssertions)
{
    cleanup();
    Table table(filename);
    InputBuffer input_buffer;
    input_buffer.set_buffer("exit");
    EXPECT_EQ(input_buffer.do_meta_command(), META_COMMAND_UNRECOGNIZED_COMMAND);
}
// test .exit command
TEST(FullTableExit, BasicAssertions)
{
    cleanup();
    Table table(filename);
    InputBuffer input_buffer;
    Statement statement;
    for(int i=0; i<1300; ++i)
    {
        std::string s = std::to_string(i);
        input_buffer.set_buffer("insert " + s + " person" + s + " email.person" + s + "@gmail.com");
        input_buffer.prepare_statement(statement);
        statement.execute_insert(table);
    }
    EXPECT_EQ(table.num_rows, MAX_TABLE_PAGES*ROWS_PER_PAGE);
    input_buffer.set_buffer(".exit");
    EXPECT_EQ(input_buffer.do_meta_command(), META_COMMAND_EXIT_SUCCESS);
    std::fstream f(filename, std::ios::out | std::ios::trunc);
    f.close();
}

TEST(BasicInsertionTest, BasicAssertions)
{
    cleanup();
    InputBuffer input_buffer;
    Statement statement;
    
    Table table(filename);
    for(size_t i=1; i<=1299; ++i)
    {
        std::string s = std::to_string(i);
        input_buffer.set_buffer("insert " + s + " person" + s + " email.person" + s + "@gmail.com");
        input_buffer.prepare_statement(statement);
        statement.execute_insert(table);
    }
    input_buffer.set_buffer("insert 1300 person1300 email.person1300@gmail.com");
    input_buffer.prepare_statement(statement);
    EXPECT_NE(statement.execute_insert(table), EXECUTE_TABLE_FULL);
    input_buffer.set_buffer("insert 1301 person1301 email.person1301@gmail.com");
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_TABLE_FULL);
}

TEST(FullTableTest, BasicAssertions)
{
    
    cleanup();
    std::string username(32, 'A');
    std::string email(255, 'M');
    InputBuffer input_buffer;
    input_buffer.set_buffer("insert 1 " + username + " " + email);
    Statement statement;
    Table table(filename);
    input_buffer.prepare_statement(statement);
    statement.execute_insert(table);
    // statement.execute_select(table); // it wasn't right so need to increase the size of our arrays by one in row.hpp which i did. 
    // So our page can contain only 13 rows maximum now.
}
TEST(TooLongStringTest, BasicAssertions)
{
    cleanup();
    std::string username(33, 'A');
    std::string email(256, 'B');
    InputBuffer input_buffer;
    input_buffer.set_buffer("insert 1 " + username + " " + email);
    Statement statement;
    Table table(filename);
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_STRING_TOO_LONG);
}



//test enter type 
TEST(EnerTypeTest, BasicAssertions)
{
    cleanup();
    std::string enter = "\n";
    InputBuffer input_buffer;
    EXPECT_FALSE(input_buffer.set_buffer(enter));
}
TEST(EmptyStringTest, BasicAssertions)
{
    cleanup();
    std:: string empty_s;
    InputBuffer input_buffer;
    EXPECT_FALSE(input_buffer.set_buffer(empty_s));
}

TEST(NotCorrectFormatedStringTest, BasicAssertions)
{
    cleanup();
    std::string s = "\n\t                                     \r";
    InputBuffer input_buffer;
    EXPECT_FALSE(input_buffer.set_buffer(s));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
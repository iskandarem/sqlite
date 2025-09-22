#include <gtest/gtest.h>
#include <InputBuffer.hpp>

TEST(DoMetaCommandTest, BasicAssertions)
{
    InputBuffer* input_buffer = new InputBuffer();
    input_buffer->set_buffer("exit");
    EXPECT_EQ(input_buffer->do_meta_command(), META_COMMAND_UNRECOGNIZED_COMMAND);
}


TEST(BasicInsertionTest, BasicAssertions)
{
    InputBuffer* input_buffer = new InputBuffer();
    Statement statement;
    Table* table = new Table();
    for(int i=0; i<1299; ++i)
    {
        std::string s = std::to_string(i);
        input_buffer->set_buffer("insert " + s + " person" + s + " email.person" + s + "@gmail.com");
        input_buffer->prepare_statement(&statement);
        statement.execute_insert(table);
    }
    input_buffer->set_buffer("insert 1299 person1299 email.person1299@gmail.com");
    input_buffer->prepare_statement(&statement);
    EXPECT_NE(statement.execute_insert(table), EXECUTE_TABLE_FULL);
    input_buffer->set_buffer("insert 1300 person1300 email.person1300@gmail.com");
    input_buffer->prepare_statement(&statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_TABLE_FULL);
}

TEST(MaxAttributeSizeTest, BasicAssertions)
{
    std::string username(32, 'A');
    std::string email(255, 'M');
    InputBuffer* input_buffer = new InputBuffer();
    input_buffer->set_buffer("insert 1 " + username + " " + email);
    Statement statement;
    Table* table = new Table();
    input_buffer->prepare_statement(&statement);
    statement.execute_insert(table);
    // statement.execute_select(table); it wasn't right so need to increase the size of our arrays by one in row.hpp which i did. 
    // So our page can contain only 13 rows maximum now.
}

//test enter type 
TEST(EnerTypeTest, BasicAssertions)
{
    std::string enter = "\n";
    InputBuffer* input_buffer = new InputBuffer();
    EXPECT_FALSE(input_buffer->set_buffer(enter));
}
TEST(EmptyStringTest, BasicAssertions)
{
    std:: string empty_s;
    InputBuffer* input_buffer = new InputBuffer();
    EXPECT_FALSE(input_buffer->set_buffer(empty_s));
}

TEST(NotCorrectFormatedStringTest, BasicAssertions)
{
    std::string s = "\n\t                                     \r";
    InputBuffer* input_buffer = new InputBuffer();
    EXPECT_FALSE(input_buffer->set_buffer(s));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
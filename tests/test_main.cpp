#include <gtest/gtest.h>
#include <InputBuffer.hpp>

TEST(DoMetaCommandTest, BasicAssertions)
{
    InputBuffer* input_buffer = new InputBuffer();
    input_buffer->set_buffer("exit");
    EXPECT_EQ(input_buffer->do_meta_command(), META_COMMAND_UNRECOGNIZED_COMMAND);
}


TEST(InsertionTest, BasicAssertions)
{
    InputBuffer* input_buffer = new InputBuffer();
    Statement statement;
    Table* table = new Table();
    for(int i=0; i<1399; ++i)
    {
        std::string s = std::to_string(i);
        input_buffer->set_buffer("insert " + s + " person" + s + " email.person" + s + "@gmail.com");
        input_buffer->prepare_statement(&statement);
        statement.execute_insert(table);
    }
    input_buffer->set_buffer("insert 1399 person1399 email.person1399@gmail.com");
    input_buffer->prepare_statement(&statement);
    EXPECT_NE(statement.execute_insert(table), EXECUTE_TABLE_FULL);
    input_buffer->set_buffer("insert 1400 person1400 email.person1400@gmail.com");
    input_buffer->prepare_statement(&statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_TABLE_FULL);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
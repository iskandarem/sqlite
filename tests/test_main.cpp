#include <gtest/gtest.h>
#include <InputBuffer.hpp>
#include <filesystem>

namespace
{
    const std::string TEST_DB = "test_db_for_tests.db";
    
    void cleanup()
    {
        if (std::filesystem::exists(TEST_DB)) {
            std::filesystem::remove(TEST_DB);
        }
    }
    
    void setup_test()
    {
        cleanup();
    }
}

// ============================================================================
// Test Fixtures for better organization and setup/teardown
// ============================================================================

class DatabaseTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        cleanup();
    }

    void TearDown() override
    {
        cleanup();
    }

    Table& get_table()
    {
        static Table table(TEST_DB);
        return table;
    }
};

TEST(DoMetaCommandTest, BasicAssertions)
{
    cleanup();
    Table table(TEST_DB);
    InputBuffer input_buffer;
    input_buffer.set_buffer("exit");
    EXPECT_EQ(input_buffer.do_meta_command(), META_COMMAND_UNRECOGNIZED_COMMAND);
}

// test .exit command
TEST(FullTableExit, BasicAssertions)
{
    cleanup();
    Table table(TEST_DB);
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
}

TEST(BasicInsertionTest, BasicAssertions)
{
    cleanup();
    InputBuffer input_buffer;
    Statement statement;
    
    Table table(TEST_DB);
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
    Table table(TEST_DB);
    input_buffer.prepare_statement(statement);
    statement.execute_insert(table);
}

TEST(TooLongStringTest, BasicAssertions)
{
    cleanup();
    std::string username(33, 'A');
    std::string email(256, 'B');
    InputBuffer input_buffer;
    input_buffer.set_buffer("insert 1 " + username + " " + email);
    Statement statement;
    Table table(TEST_DB);
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_STRING_TOO_LONG);
}

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
    std::string empty_s;
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

// ============================================================================
// MetaCommand Tests - Testing .exit and other meta commands
// ============================================================================

TEST(MetaCommandTest, ExitCommand)
{
    InputBuffer input_buffer;
    input_buffer.set_buffer(".exit");
    EXPECT_EQ(input_buffer.do_meta_command(), META_COMMAND_EXIT_SUCCESS);
}

TEST(MetaCommandTest, UnrecognizedCommand)
{
    InputBuffer input_buffer;
    input_buffer.set_buffer(".unknown");
    EXPECT_EQ(input_buffer.do_meta_command(), META_COMMAND_UNRECOGNIZED_COMMAND);
}

TEST(MetaCommandTest, InvalidMetaCommand)
{
    InputBuffer input_buffer;
    input_buffer.set_buffer(".test");
    EXPECT_EQ(input_buffer.do_meta_command(), META_COMMAND_UNRECOGNIZED_COMMAND);
}

// ============================================================================
// Input Buffer Tests - Testing buffer handling and validation
// ============================================================================

TEST(InputBufferTest, ValidInsertStatement)
{
    InputBuffer input_buffer;
    EXPECT_TRUE(input_buffer.set_buffer("insert 1 testuser testemail@example.com"));
}

TEST(InputBufferTest, ValidSelectStatement)
{
    InputBuffer input_buffer;
    EXPECT_TRUE(input_buffer.set_buffer("select"));
}

TEST(InputBufferTest, WhitespaceOnlyString)
{
    InputBuffer input_buffer;
    EXPECT_FALSE(input_buffer.set_buffer("   "));
}

TEST(InputBufferTest, TabsOnlyString)
{
    InputBuffer input_buffer;
    EXPECT_FALSE(input_buffer.set_buffer("\t\t\t"));
}

TEST(InputBufferTest, MixedWhitespace)
{
    InputBuffer input_buffer;
    EXPECT_FALSE(input_buffer.set_buffer(" \t \n \r "));
}

TEST(InputBufferTest, LeadingWhitespace)
{
    InputBuffer input_buffer;
    EXPECT_TRUE(input_buffer.set_buffer("  insert 1 user email@test.com"));
}

TEST(InputBufferTest, TrailingWhitespace)
{
    InputBuffer input_buffer;
    EXPECT_TRUE(input_buffer.set_buffer("insert 1 user email@test.com  "));
}

// ============================================================================
// Prepare Statement Tests - Testing SQL parsing
// ============================================================================

TEST(PrepareStatementTest, ValidInsert)
{
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("insert 1 testuser testemail@example.com");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_SUCCESS);
}

TEST(PrepareStatementTest, ValidSelect)
{
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("select");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_SUCCESS);
}

TEST(PrepareStatementTest, UnrecognizedStatement)
{
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("delete * from table");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_UNRECOGNIZED_STATEMENT);
}

TEST(PrepareStatementTest, InsertWithNegativeID)
{
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("insert -1 testuser testemail@example.com");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_NEGATIVE_ID);
}

TEST(PrepareStatementTest, InsertWithZeroID)
{
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("insert 0 testuser testemail@example.com");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_NEGATIVE_ID);
}

TEST(PrepareStatementTest, InsertWithUsernameExactLimit)
{
    InputBuffer input_buffer;
    Statement statement;
    std::string username(32, 'A');
    input_buffer.set_buffer("insert 1 " + username + " email@test.com");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_SUCCESS);
}

TEST(PrepareStatementTest, InsertWithUsernameTooLong)
{
    InputBuffer input_buffer;
    Statement statement;
    std::string username(33, 'A');
    input_buffer.set_buffer("insert 1 " + username + " email@test.com");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_STRING_TOO_LONG);
}

TEST(PrepareStatementTest, InsertWithEmailExactLimit)
{
    InputBuffer input_buffer;
    Statement statement;
    std::string email(255, 'A');
    input_buffer.set_buffer("insert 1 testuser " + email);
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_SUCCESS);
}

TEST(PrepareStatementTest, InsertWithEmailTooLong)
{
    InputBuffer input_buffer;
    Statement statement;
    std::string email(256, 'A');
    input_buffer.set_buffer("insert 1 testuser " + email);
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_STRING_TOO_LONG);
}

TEST(PrepareStatementTest, InsertMissingFields)
{
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("insert 1 testuser");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_SYNTAX_ERROR);
}

TEST(PrepareStatementTest, InsertWithInvalidID)
{
    InputBuffer input_buffer;
    Statement statement;
    input_buffer.set_buffer("insert abc testuser testemail@example.com");
    EXPECT_EQ(input_buffer.prepare_statement(statement), PREPARE_SYNTAX_ERROR);
}

// ============================================================================
// Row Tests - Testing row operations
// ============================================================================

class RowTest : public ::testing::Test
{
protected:
    Row row;
};

TEST_F(RowTest, SetValidID)
{
    EXPECT_TRUE(row.set_id(1));
    EXPECT_EQ(row.get_id(), 1);
}

TEST_F(RowTest, SetNegativeID)
{
    EXPECT_FALSE(row.set_id(-1));
}

TEST_F(RowTest, SetZeroID)
{
    EXPECT_FALSE(row.set_id(0));
}

TEST_F(RowTest, SetLargeID)
{
    EXPECT_TRUE(row.set_id(999999));
    EXPECT_EQ(row.get_id(), 999999);
}

TEST_F(RowTest, SetValidUsername)
{
    EXPECT_TRUE(row.set_username("testuser"));
    EXPECT_STREQ(row.get_username(), "testuser");
}

TEST_F(RowTest, SetUsernameExactLimit)
{
    std::string username(32, 'A');
    EXPECT_TRUE(row.set_username(username));
    EXPECT_STREQ(row.get_username(), username.c_str());
}

TEST_F(RowTest, SetUsernameTooLong)
{
    std::string username(33, 'A');
    EXPECT_FALSE(row.set_username(username));
}

TEST_F(RowTest, SetValidEmail)
{
    EXPECT_TRUE(row.set_email("test@example.com"));
    EXPECT_STREQ(row.get_email(), "test@example.com");
}

TEST_F(RowTest, SetEmailExactLimit)
{
    std::string email(255, 'A');
    EXPECT_TRUE(row.set_email(email));
    EXPECT_STREQ(row.get_email(), email.c_str());
}

TEST_F(RowTest, SetEmailTooLong)
{
    std::string email(256, 'A');
    EXPECT_FALSE(row.set_email(email));
}

TEST_F(RowTest, SetEmptyUsername)
{
    EXPECT_TRUE(row.set_username(""));
    EXPECT_STREQ(row.get_username(), "");
}

TEST_F(RowTest, SetEmptyEmail)
{
    EXPECT_TRUE(row.set_email(""));
    EXPECT_STREQ(row.get_email(), "");
}

TEST_F(RowTest, SetAllFields)
{
    EXPECT_TRUE(row.set_id(42));
    EXPECT_TRUE(row.set_username("john_doe"));
    EXPECT_TRUE(row.set_email("john@example.com"));
    
    EXPECT_EQ(row.get_id(), 42);
    EXPECT_STREQ(row.get_username(), "john_doe");
    EXPECT_STREQ(row.get_email(), "john@example.com");
}

// ============================================================================
// Insert and Select Tests - Testing database operations
// ============================================================================

class InsertSelectTest : public DatabaseTest
{
protected:
    InputBuffer input_buffer;
    Statement statement;
    Table table{TEST_DB};
};

TEST_F(InsertSelectTest, SingleValidInsert)
{
    input_buffer.set_buffer("insert 1 alice alice@example.com");
    input_buffer.prepare_statement(statement);
    ExecuteResult result = statement.execute_insert(table);
    EXPECT_EQ(result, EXECUTE_SUCCESS);
    EXPECT_EQ(table.num_rows, 1);
}

TEST_F(InsertSelectTest, MultipleInserts)
{
    for (int i = 1; i <= 5; ++i)
    {
        std::string cmd = "insert " + std::to_string(i) + " user" + std::to_string(i) + 
                         " user" + std::to_string(i) + "@example.com";
        input_buffer.set_buffer(cmd);
        input_buffer.prepare_statement(statement);
        EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
    }
    EXPECT_EQ(table.num_rows, 5);
}

TEST_F(InsertSelectTest, InsertWithSpecialCharacters)
{
    input_buffer.set_buffer("insert 1 user.name user+tag@example.co.uk");
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
    EXPECT_EQ(table.num_rows, 1);
}

TEST_F(InsertSelectTest, InsertWithNumbers)
{
    input_buffer.set_buffer("insert 999 user123 user123@example.com");
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
}

TEST_F(InsertSelectTest, SelectOnEmptyTable)
{
    input_buffer.set_buffer("select");
    input_buffer.prepare_statement(statement);
    ExecuteResult result = statement.execute_select(table);
    EXPECT_EQ(result, EXECUTE_SUCCESS);
}

TEST_F(InsertSelectTest, SelectAfterInsert)
{
    // Insert a row
    input_buffer.set_buffer("insert 1 testuser test@example.com");
    input_buffer.prepare_statement(statement);
    statement.execute_insert(table);
    
    // Select rows
    input_buffer.set_buffer("select");
    input_buffer.prepare_statement(statement);
    ExecuteResult result = statement.execute_select(table);
    EXPECT_EQ(result, EXECUTE_SUCCESS);
}

// ============================================================================
// Edge Cases and Boundary Tests
// ============================================================================

class EdgeCaseTest : public DatabaseTest
{
protected:
    InputBuffer input_buffer;
    Statement statement;
    Table table{TEST_DB};
};

TEST_F(EdgeCaseTest, InsertMaxSizedUsername)
{
    std::string max_username(32, 'X');
    std::string cmd = "insert 1 " + max_username + " email@test.com";
    input_buffer.set_buffer(cmd);
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
}

TEST_F(EdgeCaseTest, InsertMaxSizedEmail)
{
    std::string max_email(255, 'X');
    std::string cmd = "insert 1 user " + max_email;
    input_buffer.set_buffer(cmd);
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
}

TEST_F(EdgeCaseTest, InsertMaxSizedBoth)
{
    std::string max_username(32, 'A');
    std::string max_email(255, 'B');
    std::string cmd = "insert 1 " + max_username + " " + max_email;
    input_buffer.set_buffer(cmd);
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
}

TEST_F(EdgeCaseTest, InsertWithVeryLargeID)
{
    input_buffer.set_buffer("insert 2147483647 user email@test.com");
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
}

TEST_F(EdgeCaseTest, InsertWithUnicodeCharactersUsername)
{
    input_buffer.set_buffer("insert 1 user_äöü test@example.com");
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
}

TEST_F(EdgeCaseTest, InsertWithUnicodeCharactersEmail)
{
    input_buffer.set_buffer("insert 1 testuser tëst@exämple.com");
    input_buffer.prepare_statement(statement);
    EXPECT_EQ(statement.execute_insert(table), EXECUTE_SUCCESS);
}

// ============================================================================
// Persistence Tests - Testing data persistence
// ============================================================================

TEST(PersistenceTest, DataPersistsAcrossConnections)
{
    cleanup();
    {
        Table table1(TEST_DB);
        InputBuffer input_buffer;
        Statement statement;
        
        input_buffer.set_buffer("insert 1 alice alice@example.com");
        input_buffer.prepare_statement(statement);
        statement.execute_insert(table1);
        EXPECT_EQ(table1.num_rows, 1);
    }
    
    {
        Table table2(TEST_DB);
        EXPECT_EQ(table2.num_rows, 1);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
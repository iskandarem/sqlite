# GTest Best Practices Applied in This Project

## Test Fixture Classes

### DatabaseTest (Base Fixture)
```cpp
class DatabaseTest : public ::testing::Test
{
protected:
    void SetUp() override { cleanup(); }
    void TearDown() override { cleanup(); }
};
```
Used as base class for database-related tests.

### RowTest (Simple Fixture)
```cpp
class RowTest : public ::testing::Test
{
protected:
    Row row;  // Fresh Row object for each test
};

TEST_F(RowTest, SetValidID) { ... }
```
Uses `TEST_F` macro (instead of `TEST`) to automatically instantiate fixtures.

### InsertSelectTest & EdgeCaseTest
Inherit from `DatabaseTest` and add InputBuffer, Statement, and Table members.

## Assertion Macros Used

```cpp
EXPECT_EQ(actual, expected)           // Equality checks
EXPECT_NE(actual, expected)           // Inequality checks
EXPECT_TRUE(condition)                // Boolean true
EXPECT_FALSE(condition)               // Boolean false
EXPECT_STREQ(str1, str2)             // String comparison
```

## Test Organization Pattern

```
Namespace (anonymous)
├── Constants (TEST_DB)
└── Helper functions (cleanup)

Test Fixtures
├── DatabaseTest
├── RowTest
├── InsertSelectTest
└── EdgeCaseTest

Test Suites
├── DoMetaCommandTest
├── FullTableExit
├── BasicInsertionTest
├── FullTableTest
├── TooLongStringTest
├── EnerTypeTest
├── EmptyStringTest
├── NotCorrectFormatedStringTest
├── MetaCommandTest (3 tests)
├── InputBufferTest (7 tests)
├── PrepareStatementTest (11 tests)
├── RowTest (13 tests)
├── InsertSelectTest (6 tests)
├── EdgeCaseTest (6 tests)
└── PersistenceTest

main() - GTest Initialization
```

## Running Tests with Options

```bash
# Run all tests
./sqlite_tests

# Run with colored output
./sqlite_tests --gtest_color=yes

# Run specific test suite
./sqlite_tests --gtest_filter=RowTest.*

# Run specific test
./sqlite_tests --gtest_filter=RowTest.SetValidID

# Verbose output
./sqlite_tests --gtest_print_time=1

# List all tests without running
./sqlite_tests --gtest_list_tests
```

## Key Testing Patterns Used

### 1. Boundary Testing
```cpp
TEST_F(RowTest, SetUsernameExactLimit) {
    std::string username(32, 'A');  // Exactly at limit
    EXPECT_TRUE(row.set_username(username));
}

TEST_F(RowTest, SetUsernameTooLong) {
    std::string username(33, 'A');  // One over limit
    EXPECT_FALSE(row.set_username(username));
}
```

### 2. Setup/Cleanup with Fixtures
```cpp
class InsertSelectTest : public DatabaseTest {
protected:
    InputBuffer input_buffer;
    Statement statement;
    Table table{TEST_DB};
    // SetUp() and TearDown() inherited from DatabaseTest
};
```

### 3. Negative Testing
```cpp
TEST_F(RowTest, SetNegativeID) {
    EXPECT_FALSE(row.set_id(-1));
}

TEST(PrepareStatementTest, InsertWithNegativeID) {
    EXPECT_EQ(result, PREPARE_NEGATIVE_ID);
}
```

### 4. Integration Testing
```cpp
TEST_F(InsertSelectTest, SelectAfterInsert) {
    // Insert a row
    input_buffer.set_buffer("insert 1 testuser test@example.com");
    input_buffer.prepare_statement(statement);
    statement.execute_insert(table);
    
    // Then select
    input_buffer.set_buffer("select");
    input_buffer.prepare_statement(statement);
    ExecuteResult result = statement.execute_select(table);
    EXPECT_EQ(result, EXECUTE_SUCCESS);
}
```

## Tips for Adding More Tests

1. **Identify the component** being tested (Row, InputBuffer, Statement, Table)
2. **Choose appropriate fixture** or create new one if needed
3. **Use descriptive test names** that explain what is being tested
4. **Test one thing per test** - single responsibility
5. **Use setup fixtures** for common initialization
6. **Group related tests** in same suite using `TEST` or `TEST_F`
7. **Add comments** for complex test scenarios

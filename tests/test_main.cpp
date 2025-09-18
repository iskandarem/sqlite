#include <gtest/gtest.h>
#include <InputBuffer.hpp>

TEST(ExitCommandTest, BasicAssertions)
{
    InputBuffer* input_buffer = new InputBuffer();
    input_buffer->set_buffer("exit");
    EXPECT_EQ(input_buffer->do_meta_command(), META_COMMAND_UNRECOGNIZED_COMMAND);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
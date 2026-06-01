#include <algorithm>
#include <gtest/gtest.h>
#include <stdexcept>
#include <tsun_common/cursor.hpp>

TEST(CursorTest, PeekingTest) {
    std::array               arr{ 1, 2, 3, 4, 5 };
    tsun_common::cursor<int> cursor{ std::span<int>{ arr } };

    EXPECT_EQ(cursor.peek(), cursor.peek(0));

    EXPECT_EQ(cursor.peek(), 1);
    EXPECT_EQ(cursor.peek(1), 2);
    EXPECT_EQ(cursor.peek(2), 3);
    EXPECT_EQ(cursor.peek(3), 4);
    EXPECT_EQ(cursor.peek(4), 5);
}

TEST(CursorTest, ConsumeTest) {
    std::array               arr{ 1, 2, 3, 4, 5 };
    tsun_common::cursor<int> cursor{ std::span<int>{ arr } };

    EXPECT_EQ(cursor.position(), 0);

    auto consume0 = cursor.consume(0);
    EXPECT_EQ(consume0.size(), 0);

    EXPECT_EQ(cursor.consume(), 1);
    EXPECT_EQ(cursor.position(), 1);
    EXPECT_EQ(cursor.peek(), 2);

    std::array cmp_arr = { 2, 3 };

    EXPECT_TRUE(std::ranges::equal(std::span{ cmp_arr }, cursor.consume(2)));
    EXPECT_EQ(cursor.position(), 3);
    EXPECT_EQ(cursor.peek(), 4);

    EXPECT_NO_THROW(cursor.consume(2));
    EXPECT_FALSE(cursor.peek().has_value());

    EXPECT_THROW(cursor.consume(1), std::range_error);
}

TEST(CursorTest, RewindTest) {
    std::array               arr{ 1, 2, 3, 4, 5 };
    tsun_common::cursor<int> cursor{ std::span<int>{ arr } };

    EXPECT_FALSE(cursor.is_valid_rewind_range(1));
    EXPECT_THROW(cursor.rewind(), std::range_error);
    cursor.consume(3);
    EXPECT_EQ(cursor.rewind(), 4);

    std::array cmp_arr = { 1, 2 };

    EXPECT_TRUE(std::ranges::equal(std::span{ cmp_arr }, cursor.rewind(2)));
}

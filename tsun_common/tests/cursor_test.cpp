#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <span>
#include <stdexcept>
#include <tsun_common/cursor.hpp>

TEST(CursorTest, PeekingTest) {
    const std::array                     ARR{ 1, 2, 3, 4, 5 };
    const tsun_common::cursor<const int> CURSOR{ std::span<const int>{ ARR } };

    EXPECT_EQ(CURSOR.peek(), CURSOR.peek(0));

    EXPECT_EQ(CURSOR.peek(), 1);
    EXPECT_EQ(CURSOR.peek(1), 2);
    EXPECT_EQ(CURSOR.peek(2), 3);
    EXPECT_EQ(CURSOR.peek(3), 4);
    EXPECT_EQ(CURSOR.peek(4), 5);
}

TEST(CursorTest, ConsumeTest) {
    const std::array               ARR{ 1, 2, 3, 4, 5 };
    tsun_common::cursor<const int> cursor{ std::span<const int>{ ARR } };

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
    const std::array               ARR{ 1, 2, 3, 4, 5 };
    tsun_common::cursor<const int> cursor{ std::span<const int>{ ARR } };

    EXPECT_FALSE(cursor.is_valid_rewind_range(1));
    EXPECT_THROW(cursor.rewind(), std::range_error);
    cursor.consume(3);
    EXPECT_EQ(cursor.rewind(), 4);

    std::array cmp_arr = { 1, 2 };

    EXPECT_TRUE(std::ranges::equal(std::span{ cmp_arr }, cursor.rewind(2)));
}

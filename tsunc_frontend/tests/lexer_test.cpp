#include "tsunc_frontend/lexer.hpp"
#include "tsunc_frontend/token.hpp"

#include "gtest/gtest.h"
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

using lexer_case = std::pair<std::string, std::vector<tsunc_frontend::token>>;

// NOLINTNEXTLINE
class LexerTestingFixture : public testing::TestWithParam<lexer_case> {};

TEST_P(LexerTestingFixture, LexerTest) {
    auto [code, expected_tokens] = GetParam();

    tsunc_frontend::lexer lexer{ { "<TEST>" }, code };

    auto expected_it = expected_tokens.begin();
    auto token       = lexer.next_token();

    for (; expected_it != expected_tokens.end() && token.has_value(); ++expected_it, token = lexer.next_token()) {
        EXPECT_EQ(*expected_it, token.value());
    }

    EXPECT_TRUE(expected_it == expected_tokens.end() && !token.has_value());
}

INSTANTIATE_TEST_SUITE_P(
    EmptyTests,
    LexerTestingFixture,
    testing::Values(lexer_case("", {}), lexer_case(" ", {}), lexer_case("\t", {}), lexer_case("\n", {})));

INSTANTIATE_TEST_SUITE_P(
    KeywordTests,
    LexerTestingFixture,
    testing::Values(lexer_case(
        "func int",
        { tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::keyword_token::Func },
          tsunc_frontend::token{ "<TEST>", 5, 1, 6, tsunc_frontend::keyword_token::Int } })));

INSTANTIATE_TEST_SUITE_P(
    SymbolsTests,
    LexerTestingFixture,
    testing::Values(lexer_case(
        "-> ;{}()",
        {
            tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::symbol_token::Arrow },
            tsunc_frontend::token{ "<TEST>", 3, 1, 4, tsunc_frontend::symbol_token::Semicolon },
            tsunc_frontend::token{ "<TEST>", 4, 1, 5, tsunc_frontend::symbol_token::OpenCurly },
            tsunc_frontend::token{ "<TEST>", 5, 1, 6, tsunc_frontend::symbol_token::CloseCurly },
            tsunc_frontend::token{ "<TEST>", 6, 1, 7, tsunc_frontend::symbol_token::OpenParen },
            tsunc_frontend::token{ "<TEST>", 7, 1, 8, tsunc_frontend::symbol_token::CloseParen },
        })));

INSTANTIATE_TEST_SUITE_P(
    IdentTests,
    LexerTestingFixture,
    testing::Values(lexer_case(
        "test yoo67",
        {
            tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::ident_token{ "test" } },
            tsunc_frontend::token{ "<TEST>", 5, 1, 6, tsunc_frontend::ident_token{ "yoo67" } },
        })));

INSTANTIATE_TEST_SUITE_P(
    StringTests,
    LexerTestingFixture,
    testing::Values(
        lexer_case(R"("")", { tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::string_token{ "" } } }),
        lexer_case(R"("test")", { tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::string_token{ "test" } } }),
        lexer_case(
            R"("test\ntest\rtest\ttest\\test\"test")",
            { tsunc_frontend::token{
                "<TEST>", 0, 1, 1, tsunc_frontend::string_token{ "test\ntest\rtest\ttest\\test\"test" } } }),
        lexer_case(
            R"("test)",
            { tsunc_frontend::token{
                "<TEST>", 0, 1, 1, tsunc_frontend::unexpected_string_end_error_token("test") } })));

INSTANTIATE_TEST_SUITE_P(
    NumberTests,
    LexerTestingFixture,
    testing::Values(
        lexer_case("0", { tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::number_token{ uint64_t{ 0 } } } }),
        lexer_case(
            "67", { tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::number_token{ uint64_t{ 67 } } } })));

INSTANTIATE_TEST_SUITE_P(
    MixedTests,
    LexerTestingFixture,
    testing::Values(
        lexer_case{
            R"(func main() -> int {
0
})",
            { tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::keyword_token::Func },
              tsunc_frontend::token{ "<TEST>", 5, 1, 6, tsunc_frontend::ident_token{ "main" } },
              tsunc_frontend::token{ "<TEST>", 9, 1, 10, tsunc_frontend::symbol_token::OpenParen },
              tsunc_frontend::token{ "<TEST>", 10, 1, 11, tsunc_frontend::symbol_token::CloseParen },
              tsunc_frontend::token{ "<TEST>", 12, 1, 13, tsunc_frontend::symbol_token::Arrow },
              tsunc_frontend::token{ "<TEST>", 15, 1, 16, tsunc_frontend::keyword_token::Int },
              tsunc_frontend::token{ "<TEST>", 19, 1, 20, tsunc_frontend::symbol_token::OpenCurly },
              tsunc_frontend::token{ "<TEST>", 21, 2, 1, tsunc_frontend::number_token{ uint64_t{ 0 } } },
              tsunc_frontend::token{ "<TEST>", 23, 3, 1, tsunc_frontend::symbol_token::CloseCurly } } },
        lexer_case{
            "#",
            { tsunc_frontend::token{ "<TEST>", 0, 1, 1, tsunc_frontend::invalid_character_error_token{ '#' } } } }));

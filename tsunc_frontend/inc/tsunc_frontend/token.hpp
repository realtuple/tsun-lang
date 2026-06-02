#ifndef TSUNC_FRONTEND_TOKEN_HPP_
#define TSUNC_FRONTEND_TOKEN_HPP_

#include <cstddef>
#include <cstdint>
#include <format>
#include <ostream>
#include <string>
#include <string_view>
#include <tsun_common/source.hpp>
#include <utility>
#include <variant>

namespace tsunc_frontend {
    enum class keyword_token : std::uint8_t {
        Func,
        Int
    };

    auto to_string(keyword_token token) -> std::string;

    enum class symbol_token : std::uint8_t {
        OpenParen,
        CloseParen,
        OpenCurly,
        CloseCurly,
        Arrow,
        Colon
    };

    auto to_string(symbol_token token) -> std::string;

    struct number_token {
        using number_v = std::variant<long double, uint64_t>;

        number_v value;

        number_token(number_v value)
          : value(value) {}
        number_token(long double value)
          : value(value) {}
        number_token(uint64_t value)
          : value(value) {}

        auto operator=(number_v new_value) -> auto & {
            value = new_value;
            return *this;
        }

        auto operator=(long double new_value) -> auto & {
            value = new_value;
            return *this;
        }

        auto operator=(uint64_t new_value) -> auto & {
            value = new_value;
            return *this;
        }

        auto operator==(const number_token &) const -> bool = default;
    };

    auto to_string(number_token token) -> std::string;

    struct string_token {
        std::string value;

        string_token(std::string value)
          : value(std::move(value)) {};

        auto operator=(std::string new_value) -> auto & {
            value = std::move(new_value);
            return *this;
        }

        auto operator==(const string_token &) const -> bool = default;
    };

    auto to_string(const string_token &token) -> std::string;

    struct ident_token {
        std::string value;

        ident_token(std::string value)
          : value(std::move(value)) {};

        auto operator=(std::string new_value) -> auto & {
            value = std::move(new_value);
            return *this;
        }

        auto operator==(const ident_token &) const -> bool = default;
    };

    auto to_string(const ident_token &token) -> std::string;

    struct invalid_character_error_token {
        char character;

        auto operator==(const invalid_character_error_token &) const -> bool = default;
    };

    auto to_string(invalid_character_error_token token) -> std::string;

    struct malformed_number_error_token {
        std::string malformed_number;
        size_t      first_offset_position;

        auto operator==(const malformed_number_error_token &) const -> bool = default;
    };

    auto to_string(const malformed_number_error_token &token) -> std::string;

    struct unexpected_string_end_error_token {
        std::string string;

        auto operator==(const unexpected_string_end_error_token &) const -> bool = default;
    };

    auto to_string(const unexpected_string_end_error_token &token) -> std::string;

    using error_token =
        std::variant<invalid_character_error_token, malformed_number_error_token, unexpected_string_end_error_token>;

    auto get_error_token_type_string(const error_token &token) -> std::string;
    auto to_string(const error_token &token) -> std::string;

    using token_v = std::variant<keyword_token, symbol_token, number_token, string_token, ident_token, error_token>;

    auto get_token_v_type_string(const token_v &token) -> std::string;

    struct token {
        // Diagnostic info
        tsun_common::source_info     source_info;
        tsun_common::source_location location;

        token_v value;

        token(std::string_view source_file, size_t file_pos, size_t file_row, size_t file_column, token_v value)
          : source_info(source_file), location(file_pos, file_row, file_column), value(std::move(value)) {}

        token(tsun_common::source_info source_info, tsun_common::source_location location, token_v value)
          : source_info(source_info), location(location), value(std::move(value)) {}

        auto operator==(const token &) const -> bool = default;
    };

    auto operator<<(std::ostream &os, const tsunc_frontend::token &token) -> std::ostream &;

} // namespace tsunc_frontend

namespace std {
    template <>
    struct formatter<tsunc_frontend::token_v> {
        template <class parse_context>
        constexpr auto parse(parse_context &ctx) {
            return ctx.begin();
        }

        template <class fmt_context>
        auto format(const tsunc_frontend::token_v &token, fmt_context &ctx) const {
            return std::format_to(
                ctx.out(),
                "{} {}",
                tsunc_frontend::get_token_v_type_string(token),
                std::visit([](auto &&arg) -> std::string { return tsunc_frontend::to_string(arg); }, token));
        }
    };

    template <>
    struct formatter<tsunc_frontend::token> {
        template <class parse_context>
        constexpr auto parse(parse_context &ctx) {
            return ctx.begin();
        }

        template <class fmt_context>
        auto format(tsunc_frontend::token token, fmt_context &ctx) const {
            return std::format_to(ctx.out(), "{}:{}: {}", token.source_info.source_name, token.location, token.value);
        }
    };
} // namespace std
#endif

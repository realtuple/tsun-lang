#include <format>
#include <ostream>
#include <string>
#include <tsunc_frontend/token.hpp>
#include <variant>

namespace tsunc_frontend {
    auto to_string(keyword_token token) -> std::string {
        switch (token) {
        case keyword_token::Func: return "func";
        case keyword_token::Int: return "int";
        default: throw std::invalid_argument{ "invalid keyword_token value" };
        }
    }

    auto to_string(symbol_token token) -> std::string {
        switch (token) {
        case symbol_token::OpenParen: return "(";
        case symbol_token::CloseParen: return ")";
        case symbol_token::OpenCurly: return "{";
        case symbol_token::CloseCurly: return "}";
        case symbol_token::Arrow: return "->";
        case symbol_token::Semicolon: return ";";
        default: throw std::invalid_argument{ "invalid symbol_token value" };
        }
    }

    auto to_string(number_token token) -> std::string {
        return std::visit([](auto &&arg) -> std::string { return std::format("{}", arg); }, token.value);
    }

    auto to_string(const string_token &token) -> std::string {
        std::string str = "\"";

        for (auto ch : token.value) {
            switch (ch) {
            case '"': str += "\\\"";
            case '\n': str += "\\n";
            case '\r': str += "\\r";
            default: str += ch;
            }
        }

        return str + '"';
    }

    auto to_string(const ident_token &token) -> std::string {
        return token.value;
    }

    auto to_string(invalid_character_error_token token) -> std::string {
        std::string character;

        switch (token.character) {
        case '"': character = "\\\"";
        case '\n': character = "\\n";
        case '\r': character = "\\r";
        default: character = { 1, token.character };
        }

        return std::format("{} ({:x})", character, token.character);
    }

    auto to_string(const malformed_number_error_token &token) -> std::string {
        return std::format("{} (offset {})", token.malformed_number, token.first_offset_position);
    }

    auto to_string(const unexpected_string_end_error_token &token) -> std::string {
        std::string str = "\"";

        for (auto ch : token.string) {
            switch (ch) {
            case '"': str += "\\\"";
            case '\n': str += "\\n";
            case '\r': str += "\\r";
            default: str += ch;
            }
        }

        return str;
    }

    auto get_error_token_type_string(const error_token &token) -> std::string {
        struct visitor {
            constexpr auto operator()(invalid_character_error_token token) {
                (void)token;
                return "INVALID_CHARACTER";
            }

            constexpr auto operator()(const malformed_number_error_token &token) {
                (void)token;
                return "MALFORMED_NUMBER";
            }

            constexpr auto operator()(const unexpected_string_end_error_token &token) {
                (void)token;
                return "UNEXPECTED_END_OF_STRING";
            }
        };
        return std::visit(visitor{}, token);
    }

    auto to_string(const error_token &token) -> std::string {
        return std::visit(
            [](auto &&arg) -> std::string {
                return std::format("{} {}", get_error_token_type_string(arg), to_string(arg));
            },
            token);
    }

    auto get_token_v_type_string(const token_v &token) -> std::string {
        struct visitor {
            constexpr auto operator()(keyword_token token) -> std::string {
                (void)token;
                return "KEYWORD";
            }

            constexpr auto operator()(symbol_token token) -> std::string {
                (void)token;
                return "SYMBOL";
            }

            constexpr auto operator()(number_token token) -> std::string {
                (void)token;
                return "NUMBER";
            }

            constexpr auto operator()(const string_token &token) -> std::string {
                (void)token;
                return "STRING";
            }

            constexpr auto operator()(const ident_token &token) -> std::string {
                (void)token;
                return "IDENT";
            }

            constexpr auto operator()(const error_token &token) -> std::string {
                (void)token;
                return "ERROR";
            }
        };

        // This is the reason why rust pattern matching is better
        return std::visit(visitor{}, token);
    }

    auto operator<<(std::ostream &os, const tsunc_frontend::token &token) -> std::ostream & {
        return os << std::format("{}", token);
    }
}; // namespace tsunc_frontend

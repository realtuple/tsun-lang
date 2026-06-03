// NOLINTBEGIN(bugprone-unchecked-optional-access)

#include "tsun_common/source.hpp"

#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <tsunc_frontend/lexer.hpp>
#include <tsunc_frontend/token.hpp>
#include <utility>

namespace tsunc_frontend {
    using keyword_pair = std::pair<std::string, keyword_token>;

    constexpr static std::array S_KEYWORD_PAIRS = {
        keyword_pair{ "func", keyword_token::Func },

        keyword_pair{ "int", keyword_token::Int },
    };

    using symbol_pair = std::pair<std::string, symbol_token>;

    constexpr static std::array S_SYMBOL_PAIRS = {
        symbol_pair{ "->", symbol_token::Arrow },

        symbol_pair{ "(", symbol_token::OpenParen }, symbol_pair{ ")", symbol_token::CloseParen },
        symbol_pair{ "{", symbol_token::OpenCurly }, symbol_pair{ "}", symbol_token::CloseCurly },
        symbol_pair{ ";", symbol_token::Semicolon },
    };

    auto lexer::ms_is_first_ident_character(char character) -> bool {
        return std::isalpha(character) != 0 || character == '_';
    }

    auto lexer::ms_is_ident_character(char character) -> bool {
        return std::isalnum(character) != 0 || character == '_';
    }

    auto lexer::m_try_lexing_keyword(const std::string &keyword) -> bool {
        for (size_t i = 0; i < keyword.size(); ++i) {
            if (!m_cursor.peek(i).has_value()) return false;
            if (m_cursor.peek(i).value() != keyword.at(i)) return false;
        }
        if (!m_cursor.peek(keyword.size()).has_value()) return true;
        return !ms_is_ident_character(m_cursor.peek(keyword.size()).value());
    }

    auto lexer::m_try_lexing_keywords() -> std::optional<token> {
        for (auto pair : S_KEYWORD_PAIRS) {
            if (m_try_lexing_keyword(pair.first)) {
                token token = { m_source_info, m_cursor.position(), pair.second };
                m_cursor.consume(pair.first.size());
                return token;
            }
        }
        return {};
    }

    auto lexer::m_try_lexing_symbol(const std::string &symbol) -> bool {
        for (size_t i = 0; i < symbol.size(); ++i) {
            if (!m_cursor.peek(i).has_value()) return false;
            if (m_cursor.peek(i).value() != symbol.at(i)) return false;
        }
        return true;
    }

    auto lexer::m_try_lexing_symbols() -> std::optional<token> {
        for (auto pair : S_SYMBOL_PAIRS) {
            if (m_try_lexing_symbol(pair.first)) {
                token token = { m_source_info, m_cursor.position(), pair.second };
                m_cursor.consume(pair.first.size());
                return token;
            }
        }
        return {};
    }

    auto lexer::m_try_lexing_ident() -> std::optional<token> {
        const tsun_common::source_location LOCATION = m_cursor.position();
        std::string                        str;
        if (!ms_is_first_ident_character(m_cursor.peek().value())) return {};

        while (m_cursor.peek().has_value() && ms_is_ident_character(m_cursor.peek().value()))
            str += m_cursor.consume();

        return { { m_source_info, LOCATION, ident_token{ str } } };
    }

    auto lexer::m_try_lexing_number() -> std::optional<token> {
        const tsun_common::source_location LOCATION = m_cursor.position();
        uint64_t                           value    = 0;

        if (!m_cursor.peek().has_value() || std::isdigit(m_cursor.peek().value()) == 0) return {};

        while (m_cursor.peek().has_value() && std::isdigit(m_cursor.peek().value()) != 0) {
            value *= 10;
            value += m_cursor.consume() - '0';
        }

        return { { m_source_info, LOCATION, number_token{ value } } };
    }

    auto lexer::m_try_lexing_string() -> std::optional<token> {
        const tsun_common::source_location LOCATION = m_cursor.position();
        if (m_cursor.peek().value() != '"') return {};
        m_cursor.consume();
        std::string str;
        while (m_cursor.peek().has_value() && m_cursor.peek().value() != '"') {
            if (m_cursor.peek().value() == '\\') {
                m_cursor.consume();
                switch (m_cursor.peek().value()) {
                case 'n': str += "\n"; break;
                case 'r': str += "\r"; break;
                case 't': str += "\t"; break;
                case '\\': str += "\\"; break;
                case '"': str += "\""; break;
                default: break;
                }
                m_cursor.consume();
            } else {
                str += m_cursor.consume();
            }
        }
        if (!m_cursor.peek().has_value()) {
            return { { m_source_info, LOCATION, unexpected_string_end_error_token{ str } } };
        }
        m_cursor.consume();
        return { { m_source_info, LOCATION, string_token{ str } } };
    }

    auto lexer::next_token() -> std::optional<token> {
        while (m_cursor.peek().has_value() &&
               std::isspace(static_cast<const unsigned char>(m_cursor.peek().value())) != 0) {
            m_cursor.consume();
        }

        if (!m_cursor.peek().has_value()) return {};
        if (m_cursor.peek() == '\0') return {};

        if (auto token = m_try_lexing_keywords()) return token;
        if (auto token = m_try_lexing_symbols()) return token;
        if (auto token = m_try_lexing_ident()) return token;
        if (auto token = m_try_lexing_string()) return token;
        if (auto token = m_try_lexing_number()) return token;

        // invalid character
        token token = { m_source_info, m_cursor.position(), invalid_character_error_token{ m_cursor.peek().value() } };
        m_cursor.consume();
        return token;
    }

}; // namespace tsunc_frontend

// NOLINTEND(bugprone-unchecked-optional-access)

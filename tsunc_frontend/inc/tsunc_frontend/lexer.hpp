#ifndef TSUNC_FRONTEND_LEXER_HPP_
#define TSUNC_FRONTEND_LEXER_HPP_

#include <array>
#include <cstddef>
#include <optional>
#include <span>
#include <stack>
#include <string>
#include <tsun_common/cursor.hpp>
#include <tsun_common/source.hpp>
#include <tsunc_frontend/token.hpp>
#include <vector>

namespace tsunc_frontend {
    class lexer {
    private:
        tsun_common::source_info m_source_info;

        class cursor_wrapper {
        private:
            tsun_common::cursor<const char> m_cursor;
            tsun_common::source_location    m_location;

            std::stack<size_t> m_column_stack;
        public:
            template <typename U, size_t N>
            constexpr cursor_wrapper(const std::span<U, N> &source) noexcept
              : m_cursor(source) {};

            [[nodiscard]] auto position() const { return m_location; }
            [[nodiscard]] auto size() const { return m_cursor.size(); }

            auto is_valid_offset(size_t offset) { return m_cursor.is_valid_offset(offset); }

            auto is_valid_rewind_range(size_t range) { return m_cursor.is_valid_rewind_range(range); }

            [[nodiscard]] auto peek(size_t offset = 0) const { return m_cursor.peek(offset); }

            auto consume() {
                auto value = m_cursor.consume();
                ++m_location.offset;
                ++m_location.column;
                if (value == '\n') {
                    ++m_location.row;
                    m_column_stack.push(m_location.column);
                    m_location.column = 1;
                }
                return value;
            }

            auto consume(size_t range) -> std::span<const char> {
                const size_t ORIGNAL_POS = m_cursor.position();
                for (size_t i = 0; i < range; ++i) {
                    consume();
                }
                return m_cursor.data().subspan(ORIGNAL_POS, range);
            }

            auto rewind() -> char {
                auto value = m_cursor.rewind();
                --m_location.offset;
                --m_location.column;
                if (value == '\n') {
                    --m_location.row;
                    m_location.column = m_column_stack.top();
                    m_column_stack.pop();
                }
                return value;
            }

            auto rewind(size_t range) -> std::span<const char> {
                for (size_t i = 0; i < range; ++i) {
                    rewind();
                }
                return m_cursor.data().subspan(m_cursor.position(), range);
            }

            auto data() -> std::span<const char> { return m_cursor.data(); }
        };

        cursor_wrapper m_cursor;

        static auto ms_is_first_ident_character(char character) -> bool;
        static auto ms_is_ident_character(char character) -> bool;

        auto m_try_lexing_keyword(const std::string &keyword) -> bool;
        auto m_try_lexing_keywords() -> std::optional<token>;

        auto m_try_lexing_symbol(const std::string &symbol) -> bool;
        auto m_try_lexing_symbols() -> std::optional<token>;

        auto m_try_lexing_ident() -> std::optional<token>;

        auto m_try_lexing_number() -> std::optional<token>;

        auto m_try_lexing_string() -> std::optional<token>;

        auto m_try_handling_oneline_comment() -> bool;

        auto m_try_lexing() -> void;
    public:
        lexer(tsun_common::source_info source_info, std::span<const char> code)
          : m_source_info(source_info), m_cursor(code) {};
        lexer(tsun_common::source_info source_info, const std::vector<char> &code)
          : m_source_info(source_info), m_cursor(std::span<const char>{ code.data(), code.size() }) {};
        template <size_t N>
        lexer(tsun_common::source_info source_info, const std::array<char, N> &code)
          : m_source_info(source_info), m_cursor(code) {}

        auto next_token() -> std::optional<token>;
    };
} // namespace tsunc_frontend

#endif

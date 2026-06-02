#ifndef TSUN_COMMON_SOURCE_HPP_
#define TSUN_COMMON_SOURCE_HPP_

#include <algorithm>
#include <cstddef>
#include <format>
#include <sstream>
#include <string_view>
#include <utility>

namespace tsun_common {
    struct source_info {
        std::string_view source_name;

        source_info(std::string_view source_name)
          : source_name(source_name) {};

        auto operator==(const source_info &) const -> bool = default;
    };

    struct source_location {
        size_t offset = 0;
        size_t row    = 1;
        size_t column = 1;

        source_location() = default;

        source_location(size_t offset, size_t row, size_t column)
          : offset(offset), row(row), column(column) {}

        auto operator==(const source_location &) const -> bool = default;
    };
} // namespace tsun_common

namespace std {

    template <>
    struct formatter<tsun_common::source_location> {
        bool print_offset = false;
        bool print_row    = true;
        bool print_column = true;

        template <class parse_context>
        constexpr auto parse(parse_context &ctx) {
            auto iterator = ctx.begin();
            if (iterator == ctx.end() || *iterator == '}') return iterator;

            print_row    = false;
            print_column = false;

            // Yes, I COULD rewrite this as for loop but it is okay as while loop
            while (iterator != ctx.end() && *iterator != '}') {
                switch (*iterator) {
                case 'o': print_offset = true; break;
                case 'r': print_row = true; break;
                case 'c': print_column = true; break;
                default: throw std::format_error("Invalid format args for token_v");
                }
                ++iterator;
            }
        exit_loop:
            return iterator;
        }

        template <class fmt_context>
        auto format(tsun_common::source_location source_location, fmt_context &ctx) const {
            std::ostringstream oss;

            if (print_row) {
                oss << source_location.row;
            }

            if (print_column) {
                if (print_row) oss << ":";
                oss << source_location.column;
            }

            if (print_offset) {
                if (print_row || print_column) oss << " ";

                oss << "(offset " << source_location.offset << ")";
            }

            return std::ranges::copy(std::move(oss).str(), ctx.out()).out;
        }
    };

} // namespace std

#endif

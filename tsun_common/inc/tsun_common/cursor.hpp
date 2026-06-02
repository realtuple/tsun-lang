#ifndef TSUN_COMMON_CURSOR_HPP_
#define TSUN_COMMON_CURSOR_HPP_

#include <cstddef>
#include <functional>
#include <optional>
#include <span>
#include <stdexcept>

namespace tsun_common {
    template <typename T, std::size_t Extent = std::dynamic_extent>
    class cursor {
    private:
        std::span<T, Extent> m_span;
        std::size_t          m_pos = 0;
    public:
        cursor(const cursor &)                                = default;
        cursor(cursor &&)                                     = default;
        auto operator=(const cursor &) -> cursor<T, Extent> & = default;
        auto operator=(cursor &&) -> cursor<T, Extent> &      = default;
        virtual ~cursor()                                     = default;

        template <typename U, std::size_t N>
        explicit(Extent != std::dynamic_extent && N == std::dynamic_extent) constexpr cursor(
            const std::span<U, N> &source) noexcept
          : m_span(source){};

        auto               position() -> size_t { return m_pos; }
        [[nodiscard]] auto size() const -> size_t { return m_span.size(); }

        [[nodiscard]] auto is_valid_offset(size_t offset) const -> bool { return m_pos + offset <= m_span.size(); }

        [[nodiscard]] auto is_valid_rewind_range(size_t range) const -> bool { return m_pos >= range; }

        [[nodiscard]] auto peek(size_t offset = 0) const -> std::optional<std::reference_wrapper<const T>> {
            if (m_span.empty()) return {};
            if (offset + m_pos == m_span.size()) return {};
            return std::cref(m_span.at(m_pos + offset));
        }

        auto consume() -> T {
            if (!is_valid_offset(1)) throw std::range_error{ "can't move past last element" };
            return m_span.at(m_pos++);
        }

        auto consume(size_t range) -> std::span<T> {
            if (!is_valid_offset(range)) throw std::range_error{ "can't move past last element" };

            size_t start = m_pos;
            m_pos       += range;
            return m_span.subspan(start, range);
        }

        auto rewind() -> T {
            if (!is_valid_rewind_range(1)) throw std::range_error{ "can't rewind past first element" };
            return m_span.at(m_pos--);
        }

        auto rewind(size_t range) -> std::span<T> {
            if (!is_valid_rewind_range(range)) throw std::range_error{ "can't rewind past first element" };
            m_pos -= range;
            return m_span.subspan(m_pos, range);
        }

        auto data() -> std::span<T> { return m_span; }
    };
} // namespace tsun_common

#endif

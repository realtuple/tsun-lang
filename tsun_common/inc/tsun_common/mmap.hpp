#ifndef TSUN_COMMON_MMAP_HPP_
#define TSUN_COMMON_MMAP_HPP_

#include <cstddef>
#include <span>
#include <string_view>

namespace tsun_common {
    struct data;

    class memory_mapping final {
    private:
        char       *m_data;
        std::size_t m_size;
    public:
        memory_mapping(const memory_mapping &) = delete;
        memory_mapping(memory_mapping &&old) noexcept;
        auto operator=(const memory_mapping &) -> memory_mapping & = delete;
        auto operator=(memory_mapping &&old) noexcept -> memory_mapping &;

        memory_mapping(const std::string_view &filename);
        ~memory_mapping();

        auto data() -> std::span<const char> { return { m_data, m_size }; }
    };
} // namespace tsun_common

#endif

#include <stdexcept>
#include <string>
#include <string_view>
#include <tsun_common/mmap.hpp>

#ifdef TSUN_COMMON_HAVE_MMAP_
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace tsun_common {
    memory_mapping::memory_mapping(memory_mapping &&old) noexcept
      : m_data(old.m_data), m_size(old.m_size) {
        old.m_data = nullptr;
    }

    auto memory_mapping::operator=(memory_mapping &&old) noexcept -> memory_mapping & {
        if (this == &old) return *this;

        m_data = old.m_data;
        m_size = old.m_size;

        old.m_data = nullptr;

        return *this;
    }

    memory_mapping::memory_mapping(const std::string_view &filename) {
#ifdef TSUN_COMMON_HAVE_MMAP_
        struct stat file_stat = {};
        stat(std::string{ filename }.c_str(), &file_stat);

        int fd = open(std::string{ filename }.c_str(), O_RDONLY); // NOLINT
        m_data = static_cast<char *>(mmap(nullptr, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
        close(fd);

        if (m_data == MAP_FAILED) throw std::runtime_error{ std::string("mmap failed: ") + strerror(errno) };

        m_size = file_stat.st_size;
#endif
    }

    memory_mapping::~memory_mapping() {
#ifdef TSUN_COMMON_HAVE_MMAP_
        if (m_data != nullptr) munmap(m_data, m_size);
#endif
    }
} // namespace tsun_common

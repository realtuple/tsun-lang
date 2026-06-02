#include <string>
#include <string_view>
#include <sys/stat.h>
#include <tsun_common/mmap.hpp>

#ifdef TSUN_COMMON_HAVE_MMAP_
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace tsun_common {
    memory_mapping::memory_mapping(const std::string_view &filename) {
#ifdef TSUN_COMMON_HAVE_MMAP_
        struct stat file_stat = {};
        stat(std::string{ filename }.c_str(), &file_stat);

        int fd = open(std::string{ filename }.c_str(), O_RDONLY); // NOLINT

        m_data = static_cast<char *>(mmap(nullptr, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
        m_size = file_stat.st_size;
        close(fd);
#endif
    }

    memory_mapping::~memory_mapping() {
        munmap(m_data, m_size);
    }
} // namespace tsun_common

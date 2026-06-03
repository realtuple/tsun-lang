#include "tsun_common/mmap.hpp"

#include <cstring>
#include <print>
#include <span>
#include <tsunc_frontend/lexer.hpp>

auto main(int argc, const char **argv) -> int {
    const std::span<const char *> ARGS{ argv, static_cast<size_t>(argc) };

    tsun_common::memory_mapping mmap{ ARGS.at(1) };

    tsunc_frontend::lexer lexer{ std::string_view{ ARGS.at(1), std::strlen(ARGS.at(1)) }, mmap.data() };

    while (auto token = lexer.next_token()) {
        std::println("{}", token.value());
    }
}

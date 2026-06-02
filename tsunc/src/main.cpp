#include "tsun_common/mmap.hpp"

#include <cstring>
#include <print>
#include <tsunc_frontend/lexer.hpp>
#include <tsunc_frontend/token.hpp>

auto main(int argc, const char **argv) -> int {
    std::span<const char *> args{ argv, static_cast<size_t>(argc) };

    tsun_common::memory_mapping mmap{ args.at(1) };

    tsunc_frontend::lexer lexer{ std::string_view{ args.at(1), std::strlen(args.at(1)) }, mmap.data() };
    lexer.lex();

    for (auto token : lexer.tokens()) {
        std::println("{}", token);
    }
}

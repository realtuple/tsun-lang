#include <print>
#include <tsunc_frontend/lexer.hpp>
#include <tsunc_frontend/token.hpp>

auto main() -> int {
    tsunc_frontend::lexer lexer{ { "<TEMP>" },
                        R"(func main() -> int {
    0
})" };
    lexer.lex();
    for (auto token : lexer.tokens()) {
        std::println("{}", token);
    }
}

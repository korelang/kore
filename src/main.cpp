#include <iostream>

#include "ast/ast_stream_writer.hpp"
#include "logging/logging.hpp"
#include "parser.hpp"
#include "token.hpp"

const std::string COMPILER_NAME = "korec";

struct Version {
    int major;
    int minor;
    int patch;
};

const Version CURRENT_VERSION{ 0, 1, 0 };

int main(int argc, char** argv) {
    if (argc != 2) {
        error("%s requires one argument", COMPILER_NAME.c_str());
        return 1;
    }

    info(
        "%s v%d.%d.%d",
        COMPILER_NAME.c_str(),
        CURRENT_VERSION.major,
        CURRENT_VERSION.minor,
        CURRENT_VERSION.patch
    );

    info("Compiling '%s'", argv[1]);

    // 1. Parse input file
    Parser parser{};

    Ast ast;

    try {
        parser.parse_file(argv[1], &ast);
    } catch (std::runtime_error& ex) {
        return 1;
    }

    AstStreamWriter stream_writer{std::cerr};

    if (!parser.failed()) {
        stream_writer.write(ast);
    }

    // 2. Check functions have a return statement

    // 3. Infer types

    // 4. Check types

    /* if (typecheck_only) { */
    /*     return 0; */
    /* } */

    // 5. Eliminate dead code

    // 6. Constant folding

    // 7. Add explicit type conversions

    /* if (dump_ast) { */
    /*     dump_ast(ast); */
    /* } */

    return 0;
}

#include <iostream>

#include "ast/ast_stream_writer.hpp"
#include "logging/logging.hpp"
#include "options.hpp"
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
    auto args = parse_commandline(argc, argv);

    if (args.error_message.size() > 0) {
        error("%s", args.error_message.c_str());
        return 1;
    }

    if (args.version) {
        info(
            "%s v%d.%d.%d",
            COMPILER_NAME.c_str(),
            CURRENT_VERSION.major,
            CURRENT_VERSION.minor,
            CURRENT_VERSION.patch
        );

        return 0;
    } else if (args.version_only) {
        std::cout <<
            CURRENT_VERSION.major << "." <<
            CURRENT_VERSION.minor << "." <<
            CURRENT_VERSION.patch << std::endl;

        return 0;
    } else if (args.help) {
        print_help_message();
        return 0;
    }

    info("Compiling '%s'", argv[1]);

    // 1. Parse input file
    Parser parser{};

    Ast ast;

    try {
        parser.parse_file(argv[1], &ast);
    } catch (std::runtime_error& ex) {
        return 1;
    }

    if (!parser.failed()) {
        if (args.dump_parse) {
            AstStreamWriter stream_writer{std::cerr};
            stream_writer.write(ast);
        }

        success("%d errors", parser.error_count());
    } else {
        error("%d errors", parser.error_count());
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

    return 0;
}

#include <iostream>

#include "ast/ast_stream_writer.hpp"
#include "logging/logging.hpp"
#include "options.hpp"
#include "parser.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "types/type_checker.hpp"
#include "types/type_inferrer.hpp"

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
    } else if (args.dump_scan) {
        // Dump all scanned tokens to stderr
        Scanner scanner{};

        if (!scanner.open_file(args.filename)) {
            std::cerr << "Failed to open file" << std::endl;
            return 1;
        }

        while (!scanner.eof()) {
            std::cerr << scanner.next_token() << std::endl;
        }

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
    } else {
        error("%d errors", parser.error_count());
    }

    success(1, args.verbosity, "parse successful");

    // 2. Check functions have a return statement

    // 3. Infer types
    TypeInferrer type_inferrer;

    type_inferrer.infer(ast);

    success(1, args.verbosity, "type inference successful");

    // 4. Check types
    TypeChecker type_checker;

    int error_count = type_checker.check(ast);

    if (error_count > 0) {
        error_group("typecheck", "%d type errors", error_count);

        for (auto& type_error : type_checker.errors()) {
            std::ostringstream oss;

            oss << type_error.location;

            error_indent(
                "[%s]: %s",
                oss.str().c_str(),
                type_error.message.c_str()
            );
        }
    } else {
        success(1, args.verbosity, "typecheck successful");
    }

    if (args.typecheck_only || error_count > 0) {
        return error_count > 0 ? 1 : 0;
    }

    // 7. Add explicit type conversions

    return 0;
}

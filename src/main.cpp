#include <array>
#include <iostream>
#include <sstream>

#include "backend/bytecode_format_writer.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
#include "backend/vm/vm.hpp"
#include "compiler.hpp"
#include "debug_time.hpp"
#include "decode_instruction.hpp"
#include "frontend/ast/ast_stream_writer.hpp"
#include "logging/logging.hpp"
#include "options.hpp"
#include "parser.hpp"
#include "passes/passes.hpp"
#include "passes/passes.hpp"
#include "pointer_types.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "types/scope.hpp"
#include "types/type_checker.hpp"
#include "types/type_inferrer.hpp"
#include "version.hpp"

namespace kore {
    const std::string COMPILER_NAME = "korec";
    const Version CURRENT_VERSION{ 0, 1, 0 };

    void print_version(Version version, bool version_only) {
        if (version_only) {
            std::cout << version << std::endl;
        } else {
            info(
                "%s v%d.%d.%d",
                COMPILER_NAME.c_str(),
                version.major,
                version.minor,
                version.patch
            );
        }
    }

    int dump_tokens(
        bool execute,
        const std::string& expr,
        const std::string& filename
    ) {
        // Dump all scanned tokens to stderr
        Scanner scanner{};
        Token token;

        debug_group("scan", "dumping scanned tokens");

        if (execute) {
            scanner.scan_string(expr);
        } else {
            if (!scanner.open_file(filename)) {
                error_group("scan", "failed to open file: '%s'", filename.c_str());
                return 1;
            }
        }

        do {
            token = scanner.next_token();
            token.column_format(std::cerr) << std::endl;
        } while (!token.is_eof());

        return 0;
    }

    int dump_parse(
        bool execute,
        const std::string& expr,
        const std::string& filename
    ) {
        Ast ast;
        Parser parser;

        if (execute) {
            debug_time("parse", [&expr, &ast, &parser](){ parser.parse_non_module(expr, &ast); });
        } else {
            debug_time("parse", [&filename, &ast, &parser](){ parser.parse_file(filename, &ast); });
        }

        if (!parser.failed()) {
            debug_group("parse", "dumping ast");
            AstStreamWriter stream_writer{std::cerr};
            stream_writer.write(ast);
        } else {
            error_group("parse", "parse failed with %d errors", parser.error_count());
            return 1;
        }

        return 0;
    }
}

int main(int argc, char** argv) {
    auto args = kore::parse_commandline(argc, argv);

    if (args.error_message.size() > 0) {
        kore::error("%s", args.error_message.c_str());
        return 1;
    }

    if (args.version) {
        kore::print_version(kore::CURRENT_VERSION, false);
        return 0;
    } else if (args.version_only) {
        kore::print_version(kore::CURRENT_VERSION, true);
        return 0;
    } else if (args.help) {
        kore::print_help_message();
        return 0;
    } else if (args.dump_scan) {
        // Dump all scanned tokens to stderr
        return kore::dump_tokens(args.execute, args.expr, args.filename);
    }

    try {
        kore::Compiler compiler{args, kore::get_default_passes()};

        return compiler.run_passes();
    } catch (std::runtime_error& ex) {
        kore::error_group("fatal", "%s", ex.what());

        return 1;
    }
}

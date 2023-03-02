#include <array>
#include <filesystem>
#include <iostream>
#include <sstream>

#include "ast/ast_element_stream_writer.hpp"
#include "ast/ast_stream_writer.hpp"
#include "backend/bytecode_format_writer.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
#include "backend/vm/vm.hpp"
#include "compiler.hpp"
#include "debug_time.hpp"
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

namespace fs = std::filesystem;

namespace kore {
    const std::string COMPILER_NAME = "korec";
    const Version CURRENT_VERSION{ 0, 1, 0 };

    int dump_tokens(
        bool execute,
        const std::string& expr,
        const fs::path& path
    ) {
        // Dump all scanned tokens to stderr
        Scanner scanner{};
        Token token;

        debug_group("scan", "dumping scanned tokens");

        if (execute) {
            scanner.scan_string(expr);
        } else {
            if (!scanner.open_file(path)) {
                error_group("scan", "failed to open file: '%s'", path.c_str());
                return 1;
            }
        }

        for (auto& token : scanner) {
            token.column_format(std::cerr) << std::endl;
        }

        return 0;
    }

    int dump_parse_helper(
        bool execute,
        const std::string& expr,
        fs::path& path,
        AstWriter& writer
    ) {
        Ast ast;
        Parser parser;
        auto group = "parse";

        if (execute) {
            debug_time(group, [&expr, &ast, &parser](){ parser.parse_non_module(expr, &ast); });
        } else {
            debug_time(group, [&path, &ast, &parser](){ parser.parse_file(path, &ast); });
        }

        if (!parser.failed()) {
            writer.write(ast);
        } else {
            error_group(group, "parse failed with %d errors", parser.error_count());
            return 1;
        }

        return 0;
    }

    int dump_parse(bool execute, const std::string& expr, fs::path& path) {
        debug_group("parse", "dumping ast");
        AstStreamWriter stream_writer{std::cerr};

        return dump_parse_helper(execute, expr, path, stream_writer);
    }

    int dump_parse_raw(bool execute, const std::string& expr, fs::path& path) {
        debug_group("parse", "dumping parse");
        AstElementStreamWriter stream_writer{std::cerr};

        return dump_parse_helper(execute, expr, path, stream_writer);
    }
}

int main(int argc, char** argv) {
    auto args = kore::parse_commandline(argc, argv);

    if (args.error_message.size() > 0) {
        kore::error("%s", args.error_message.c_str());
        return 1;
    }

    if (args.version || args.version_only) {
        kore::print_version(kore::COMPILER_NAME, kore::CURRENT_VERSION, args.version_only);
        return 0;
    } else if (args.help) {
        kore::print_help_message();
        return 0;
    } else if (args.dump_scan) {
        // Dump all scanned tokens to stderr
        return kore::dump_tokens(args.execute, args.expr, args.path);
    } else if (args.dump_parse) {
        return kore::dump_parse_raw(args.execute, args.expr, args.path);
    } else if (args.dump_ast) {
        return kore::dump_parse(args.execute, args.expr, args.path);
    }

    try {
        kore::Compiler compiler{args, kore::get_default_passes()};

        return compiler.run_passes();
    } catch (std::runtime_error& ex) {
        kore::error_group("fatal", "%s", ex.what());

        return 1;
    }
}

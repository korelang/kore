#include <array>
#include <filesystem>
#include <iostream>
#include <sstream>

#include "ast/ast_element_stream_writer.hpp"
#include "ast/ast_stream_writer.hpp"
#include "ast/parser/parser.hpp"
#include "ast/scanner/scanner.hpp"
#include "ast/scanner/token.hpp"
#include "bin/korec/options.hpp"
#include "compiler/compiler.hpp"
#include "compiler/passes/passes.hpp"
#include "compiler/passes/passes.hpp"
#include "logging/logging.hpp"
#include "pointer_types.hpp"
#include "targets/bytecode/bytecode_format_writer.hpp"
#include "targets/bytecode/codegen/bytecode_codegen.hpp"
#include "targets/bytecode/vm/vm.hpp"
#include "types/scope.hpp"
#include "types/type_checker.hpp"
#include "types/type_inferrer.hpp"
#include "utils/debug_time.hpp"
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

        debug_group("scan", path.c_str());

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
        AstWriter& writer,
        const ParsedCommandLineArgs& args
    ) {
        Ast ast{path};
        Parser parser;
        auto group = "parse";

        if (execute) {
            debug_time(group, [&expr, &ast, &parser, &args](){ parser.parse_non_module(expr, &ast, args); });
        } else {
            debug_time(group, [&path, &ast, &parser, &args](){ parser.parse_file(path, &ast, args); });
        }

        if (!parser.failed()) {
            writer.write(ast);
        } else {
            error_group(group, "parse failed with %d errors", parser.error_count());
            return 1;
        }

        return 0;
    }

    int dump_parse(bool execute, const std::string& expr, fs::path& path, const ParsedCommandLineArgs& args) {
        AstStreamWriter stream_writer{std::cerr};

        return dump_parse_helper(execute, expr, path, stream_writer, args);
    }

    int dump_parse_raw(bool execute, const std::string& expr, fs::path& path, const ParsedCommandLineArgs& args) {
        AstElementStreamWriter stream_writer{std::cerr};

        return dump_parse_helper(execute, expr, path, stream_writer, args);
    }

    void dump_kir(const kir::Kir& kir) {
        for (auto module = kir.cbegin(); module < kir.cend(); ++module) {
            for (auto func = module->cbegin(); func < module->cend(); ++func) {
                debug_group(
                    "kir",
                    "function %s of module %d at %s",
                    func->name().c_str(),
                    module->index(),
                    module->path().c_str()
                );
                func->graph().write_adjacency_lists(std::cerr);
                std::cerr << std::endl;
            }
        }
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
    } else if (args.dump == kore::DumpOption::Scan) {
        kore::debug_group("scan", "dumping scanned tokens");

        for (auto& path : args.paths) {
            kore::dump_tokens(args.execute, args.expr, path);
        }

        return 0;
    } else if (args.dump == kore::DumpOption::Parse) {
        kore::debug_group("scan", "dumping parse");

        for (auto& path : args.paths) {
            kore::dump_parse_raw(args.execute, args.expr, path, args);
        }

        return 0;
    }

    try {
        kore::Compiler compiler{args, kore::get_default_passes()};

        auto ret_code = compiler.run_passes();

        if (args.dump == kore::DumpOption::Kir) {
            kore::debug_group("kir", "dumping kir graph");
            kore::dump_kir(compiler.context().kir);
        }

        return ret_code;
    } catch (std::runtime_error& ex) {
        kore::error_group("fatal", "%s", ex.what());

        return 1;
    }
}

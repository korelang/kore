#include <array>
#include <iostream>
#include <sstream>

#include "frontend/ast/ast_stream_writer.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
#include "backend/vm/vm.hpp"
#include "decode_instruction.hpp"
#include "debug_time.hpp"
#include "logging/logging.hpp"
#include "options.hpp"
#include "parser.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "types/scope.hpp"
#include "types/type_checker.hpp"
#include "types/type_inferrer.hpp"
#include "version.hpp"

namespace kore {
    const std::string COMPILER_NAME = "korec";
    const Version CURRENT_VERSION{ 0, 1, 0 };

    void print_version(bool version_only) {
        if (version_only) {
            info(
                "%s v%d.%d.%d",
                COMPILER_NAME.c_str(),
                CURRENT_VERSION.major,
                CURRENT_VERSION.minor,
                CURRENT_VERSION.patch
            );
        } else {
            std::cout
                << CURRENT_VERSION.major << "."
                << CURRENT_VERSION.minor << "."
                << CURRENT_VERSION.patch
                << std::endl;
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

        if (execute) {
            scanner.scan_string(expr);
        } else {
            if (!scanner.open_file(filename)) {
                std::cerr << "Failed to open file" << std::endl;
                return 1;
            }
        }

        try {
            debug_group("scan", "dumping scanned tokens");

            do {
                token = scanner.next_token();
                token.column_format(std::cerr) << std::endl;
            } while (!token.is_eof());
        } catch (const std::runtime_error& ex) {
            return 1;
        }

        return 0;
    }

    int parse_input(Parser& parser, Ast& ast, const ParsedCommandLineArgs& args) {
        try {
            if (args.execute) {
                debug_time("Parse", [&args, &ast, &parser](){ parser.parse_non_module(args.expr, &ast); });
            } else {
                debug_time("Parse", [&args, &ast, &parser](){ parser.parse_file(args.filename, &ast); });
            }
        } catch (std::runtime_error& ex) {
            return 1;
        }

        if (!parser.failed()) {
            if (args.dump_parse) {
                debug_group("parse", "dumping ast");
                AstStreamWriter stream_writer{std::cerr};
                stream_writer.write(ast);
                return 0;
            }
        } else {
            error("%d errors", parser.error_count());
            return 1;
        }

        return 0;
    }

    void infer_types(Ast& ast) {
        TypeInferrer type_inferrer;

        type_inferrer.infer(ast);
    }

    int check_types(const std::string& source_name, const Ast& ast, int verbosity) {
        TypeChecker type_checker;

        int error_count = type_checker.check(ast);

        if (error_count > 0) {
            error_group("typecheck", "%d type errors", error_count);

            for (auto& type_error : type_checker.errors()) {
                std::ostringstream oss;

                oss << type_error.location.colon_format();

                error_indent(
                    "[%s:%s]: %s",
                    source_name.c_str(),
                    oss.str().c_str(),
                    type_error.message.c_str()
                );
            }
        } else {
            success_group(1, verbosity, "typecheck", "");
        }

        return error_count;
    }

    int run(int argc, char** argv) {
        auto args = parse_commandline(argc, argv);

        if (args.error_message.size() > 0) {
            error("%s", args.error_message.c_str());
            return 1;
        }

        if (args.version) {
            print_version(false);
            return 0;
        } else if (args.version_only) {
            print_version(true);
            return 0;
        } else if (args.help) {
            print_help_message();
            return 0;
        } else if (args.dump_scan) {
            // Dump all scanned tokens to stderr
            return dump_tokens(args.execute, args.expr, args.filename);
        }

        std::string source_name = args.execute ? "<string>" : args.filename;

        // 1. Parse input file
        Parser parser{};
        Ast ast;

        int result = parse_input(parser, ast, args);

        if (result != 0 || (result == 0 && args.dump_parse)) {
            return result;
        }

        success_group(1, args.verbosity, "parser", "");

        // 2. Check functions have a return statement


        // 3. Infer types
        infer_types(ast);
        success_group(1, args.verbosity, "type inference", "");

        // 4. Check types
        int error_count = check_types(source_name, ast, args.verbosity);

        if (args.typecheck_only || error_count > 0) {
            return error_count > 0 ? 1 : 0;
        }

        // 5. Optimisations
        /* Optimiser optimiser{}; */

        // 5a. Eliminate dead code
        /* optimiser.optimise_dead_code(); */

        /* success_group("optimisation", "dead code"); */

        // 5b. Constant folding
        /* optimiser.optimise_constant_folding(); */

        /* success_group(2, args.verbosity, "optimisation", "constant folding"); */

        /* success(1, args.verbosity, "optimisation successful"); */

        // 6. Add explicit type conversions

        // 7. Compilation
        BytecodeGenerator code_generator;
        auto module = code_generator.compile(ast);
        success_group(1, args.verbosity, "compilation", "");

            return 0;
        }

        // 8. Run code
        /* Vm vm; */
        /* std::vector<bytecode_type> code{code_generator.begin(), code_generator.end()}; */
        /* vm.run(code); */

        return 0;
    }
}

int main(int argc, char** argv) {
    try {
        return kore::run(argc, argv);
    } catch (std::runtime_error& ex) {
        kore::error_group("fatal", "%s", ex.what());

        return 1;
    }
}

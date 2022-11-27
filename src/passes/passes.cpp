#include "config.hpp"
#include "passes/passes.hpp"
#include "bytecode_format_writer.hpp"
#include "codegen/bytecode/bytecode_codegen.hpp"
#include "parser.hpp"
#include "types/type_checker.hpp"
#include "types/type_inferrer.hpp"

namespace kore {
    Pass get_parser_pass() {
        return Pass{
            "parser",
            [](PassContext& context) {
                Parser parser;
                parser.parse_file(context.cmdline_args.path, &context.ast); 

                return PassResult{ !parser.failed(), {} };
            }
        };
    }

    Pass get_type_inference_pass() {
        return Pass{
            "type inference",
            [](PassContext& context) {
                TypeInferrer type_inferrer;
                type_inferrer.infer(context.ast); 

                return PassResult{ true, {} };
            }
        };
    }

    Pass get_type_checking_pass() {
        return Pass{
            "type checking",
            [](PassContext& context) {
                TypeChecker type_checker;
                int error_count = type_checker.check(context.ast);

                return PassResult{ error_count == 0, type_checker.errors() };
            }
        };
    }

    Pass get_bytecode_codegen_pass() {
        return Pass {
            "bytecode generation",
            [](PassContext& context) {
                BytecodeGenerator code_generator;
                context.module = code_generator.compile(context.ast);

                return PassResult{ true, {} };
            }
        };
    }

    Pass get_bytecode_write_pass() {
        return Pass {
            "bytecode write",
            [](PassContext& context) {
                BytecodeFormatWriter writer;
                writer.write(
                    context.module.get(),
                    context.cmdline_args.path.replace_extension(
                        config::KORE_COMPILED_EXTENSION
                    )
                );

                return PassResult{ true, {} };
            }
        };
    }

    std::vector<Pass> get_default_passes() {
        return {
            get_parser_pass(),
            get_type_inference_pass(),
            get_type_checking_pass(),
            get_bytecode_codegen_pass(),
            get_bytecode_write_pass()
        };
    }
}

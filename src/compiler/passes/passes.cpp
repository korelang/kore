#include "compiler/config.hpp"
#include "compiler/passes/passes.hpp"
#include "targets/bytecode/bytecode_format_writer.hpp"
#include "targets/bytecode/codegen/bytecode_codegen.hpp"
#include "ast/parser/parser.hpp"
#include "types/type_checker.hpp"
#include "types/type_inferrer.hpp"

namespace kore {
    Pass get_parser_pass() {
        return Pass{
            "parser",
            [](PassContext& context) {
                Parser parser;

                for (const auto& path : context.args.paths) {
                    Ast ast;
                    parser.parse_file(path, &ast); 
                    context.asts.push_back(std::move(ast));
                }

                // TODO: Add errors
                return PassResult{ !parser.failed(), {} };
            }
        };
    }

    Pass get_type_inference_pass() {
        return Pass{
            "type inference",
            [](PassContext& context) {
                TypeInferrer type_inferrer;

                for (auto& ast : context.asts) {
                    type_inferrer.infer(ast); 
                }

                return PassResult{ true, {} };
            }
        };
    }

    Pass get_type_checking_pass() {
        return Pass{
            "type checking",
            [](PassContext& context) {
                TypeChecker type_checker;
                int error_count = 0;

                for (auto& ast : context.asts) {
                    error_count += type_checker.check(ast);
                }

                return PassResult{ error_count == 0, type_checker.errors() };
            }
        };
    }

    Pass get_bytecode_codegen_pass() {
        return Pass {
            "codegen:bytecode",
            [](PassContext& context) {
                BytecodeGenerator code_generator;

                for (auto it = context.kir.begin(); it < context.kir.end(); ++it) {
                    /* context.buffers.push_back(code_generator.generate(*it)); */
                }

                // Stop running passes here if --dump-kir was passed
                return PassResult{ context.args.dump_kir.empty(), {} };
            }
        };
    }

    Pass get_bytecode_write_pass() {
        return Pass {
            "bytecode write",
            [](PassContext& context) {
                for (std::size_t idx = 0; idx < context.args.paths.size(); ++idx) {
                    auto& path = context.args.paths[idx];
                    auto& target_path = path.replace_extension(config::KORE_COMPILED_EXTENSION);
                    auto& buffer = context.buffers[idx];
                    std::ofstream ofs(target_path, std::ios::out | std::ios::binary);

                    ofs.write(
                        reinterpret_cast<const char*>(buffer.data()),
                        buffer.size()
                    );
                }

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

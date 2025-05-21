#include <fstream>

#include "bin/korec/options.hpp"
#include "compiler/config.hpp"
#include "compiler/passes/passes.hpp"
#include "targets/bytecode/codegen/bytecode_generator.hpp"
#include "targets/bytecode/passes/kir_pass.hpp"
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
                    auto ast = parser.parse_file(path, context.args); 

                    if (!ast.has_value()) {
                        return PassResult{ !parser.failed() && parser.error_count() == 0, {} };
                    }

                    context.asts.push_back(std::move(ast.value()));
                }

                // TODO: Add errors
                return PassResult{ !parser.failed() && parser.error_count() == 0, {} };
            }
        };
    }

    Pass get_type_inference_pass() {
        return Pass{
            "type inference",
            [](PassContext& context) {
                TypeInferrer type_inferrer(context.args);

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
                TypeChecker type_checker(context.args);
                int error_count = 0;

                for (auto& ast : context.asts) {
                    error_count += type_checker.check(ast);
                }

                return PassResult{ error_count == 0, type_checker.diagnostics() };
            }
        };
    }

    Pass get_bytecode_codegen_pass() {
        return Pass {
            "codegen:bytecode",
            [](PassContext& context) {
                BytecodeGenerator code_generator;

                context.buffers.push_back(code_generator.generate(context.kir));

                bool proceed = context.args.dump == DumpOption::None || context.args.dump != DumpOption::Kir;

                // Stop running passes here if we are dumping kir
                return PassResult{ proceed, {} };
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

                    ofs.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
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
            kir::get_kir_lowering_pass(),
            get_bytecode_codegen_pass(),
            get_bytecode_write_pass()
        };
    }
}

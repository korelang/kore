#include "targets/bytecode/codegen/kir/kir.hpp"
#include "targets/bytecode/codegen/kir/kir_lowering_pass.hpp"
#include "targets/bytecode/passes/kir_pass.hpp"

namespace kore {
    namespace kir {
        Pass get_kir_lowering_pass() {
            return Pass {
                "kir",
                [](PassContext& context) {
                    KirLoweringPass kir_pass(context.args);

                    // TODO: Set main module index
                    for (auto& ast : context.asts) {
                        auto module = kir_pass.lower(ast);
                        context.kir.add_module(module);
                    }

                    /* auto errors = kir_pass.errors(); */

                    /* return PassResult{ errors.size() == 0, errors }; */
                    return PassResult{ true, {} };
                }
            };
        }
    }
}

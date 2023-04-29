#ifndef KORE_PASS_CONTEXT_HPP
#define KORE_PASS_CONTEXT_HPP

#include <filesystem>

#include "ast/ast.hpp"
#include "targets/bytecode/module.hpp"
#include "bin/korec/options.hpp"
#include "targets/bytecode/codegen/kir/kir.hpp"

namespace kore {
    /// Context to propagate through compiler passes
    struct PassContext {
        PassContext() = default;
        PassContext(ParsedCommandLineArgs args) : args(args) {}

        // Command line arguments
        ParsedCommandLineArgs args;

        // TODO: Make the parser produce an AST instead
        // List of ASTs produced from each path
        std::vector<Ast> asts;

        // KIR representation for each AST
        kir::Kir kir;

        // List of bytecode buffers for each AST
        std::vector<std::vector<std::uint8_t>> buffers;
    };
}

#endif // KORE_PASS_CONTEXT_HPP

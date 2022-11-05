#ifndef KORE_PASS_CONTEXT_HPP
#define KORE_PASS_CONTEXT_HPP

#include "ast/ast.hpp"
#include "module.hpp"
#include "options.hpp"

namespace kore {
    /// Context to propagate through compiler passes
    struct PassContext {
        PassContext() = default;
        PassContext(ParsedCommandLineArgs args) : cmdline_args(args) {}

        std::string source_name;

        Ast ast; // TODO: Make the parser produce an AST instead
        ParsedCommandLineArgs cmdline_args;
        Owned<Module> module;
    };
}

#endif // KORE_PASS_CONTEXT_HPP

#include "compiler.hpp"
#include "logging/logging.hpp"
#include "passes/pass_result.hpp"

namespace kore {
    Compiler::Compiler(
        ParsedCommandLineArgs args,
        const std::vector<Pass>& passes
    ) : _context(args), _passes(passes) {}

    int Compiler::run_passes() {
        auto verbosity = _context.cmdline_args.verbosity;

        for (auto& pass : _passes) {
            auto result = pass.run(_context);

            if (!result.proceed && result.errors.size() > 0) {
                print_errors(pass, result);
                return 1;
            } else {
                kore::success_group(1, verbosity, pass.name, "");
            }

            if (!result.proceed) {
                // Some passes may not have any errors but may exit the pass run early
                break;
            }
        }

        return 0;
    }

    void Compiler::print_errors(const Pass& pass, PassResult result) {
        error_group(pass.name, "%d %s errors", result.errors.size(), "");

        for (const auto& error : result.errors) {
            section_error(pass.name, error, 1);
        }
    }
}

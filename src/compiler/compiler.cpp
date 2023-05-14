#include "compiler.hpp"
#include "logging/logging.hpp"
#include "passes/pass_result.hpp"

namespace kore {
    Compiler::Compiler(
        ParsedCommandLineArgs args,
        const std::vector<Pass>& passes
    ) : _context(args), _passes(passes) {}

    int Compiler::run_passes() {
        auto verbosity = _context.args.verbosity;

        for (auto& pass : _passes) {
            kore::info_group(1, verbosity, "compiler", "starting pass '%s'", pass.name.c_str());
            auto result = pass.run(_context);
            kore::info_group(1, verbosity, "compiler", "end of pass '%s'", pass.name.c_str());

            // TODO: We can do per-pass timings here
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

    const PassContext& Compiler::context() const {
        return _context;
    }

    void Compiler::print_errors(const Pass& pass, PassResult result) {
        error_group(pass.name, "%d %serrors", result.errors.size(), "");

        for (const auto& error : result.errors) {
            section_error(pass.name, error, 1);
        }
    }
}

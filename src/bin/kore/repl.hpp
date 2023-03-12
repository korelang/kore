#ifndef KORE_REPL_HPP
#define KORE_REPL_HPP

#include <string>

namespace kore {
    class Repl final {
        public:
            Repl() = default;
            virtual ~Repl();

            void run();

        private:
            bool _running;

            void start();
            void stop();
            bool is_running();
            bool handle_command(const std::string& input);
            void print_help();

            static std::string _prompt;
    };
}

#endif // KORE_REPL_HPP

#include "bin/kore/repl.hpp"
#include "logging/logging.hpp"

#include <iostream>

namespace kore {
    Repl::~Repl() {}

    void Repl::run() {
        std::string input = "";

        std::cout << "kore repl" << std::endl << std::endl
            << "Type '@help' for help" << std::endl
            << "Type '@exit' or '@quit' to exit" << std::endl;

        start();

        while (is_running()) {
            std::cout << Repl::_prompt << " ";
            std::getline(std::cin, input);

            if (handle_command(input)) {
                continue;
            }
        }
    }

    void Repl::start() {
        _running = true;
    }

    void Repl::stop() {
        _running = false;
    }

    bool Repl::is_running() {
        return _running;
    }

    bool Repl::handle_command(const std::string& input) {
        if (input.empty() || input[0] != '@') {
            return false;
        }

        if (input == "@help") {
            print_help();
        } else if (input == "@version") {
            std::cout << "version ???" << std::endl;
        } else if (input == "@exit" || input == "@quit") {
            stop();
        } else {
            error("Unknown command '%s'", input.c_str());
        }

        return true;
    }

    void Repl::print_help() {
        std::cout
            << "Type '@help' for help" << std::endl
            << "Type '@version' for version info" << std::endl
            << "Type '@exit' or '@quit' to exit" << std::endl
            << "Type a kore expression to evaluate it" << std::endl;
    }

    std::string Repl::_prompt = ">>";
}

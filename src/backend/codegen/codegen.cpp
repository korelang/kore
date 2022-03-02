#include "backend/codegen/codegen.hpp"

namespace kore {
    Codegen::Codegen() {}

    Codegen::~Codegen() {}

    void Codegen::set_writer(OpcodeWriter* writer) {
        this->_writer = writer;
    }

    void Codegen::compile(const Ast& ast) {
        if (!out) {
            throw std::runtime_error("Output stream not set for compilation");
        }

        emit_header();
        emit_version();

        for (auto const& statement : ast) {
            statement->accept(this);
        }
    }

    void Codegen::emit_version() {}

    void Codegen::emit_header() {
    /*     /1* out->write("kore", 4); *1/ */
    }

    void Codegen::emit() {}

    /* void Codegen::write(const std::string& value) { */
    /*     out->write(value.c_str(), value.size()); */
    /* } */
}

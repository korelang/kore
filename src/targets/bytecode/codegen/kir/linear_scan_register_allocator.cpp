#include "targets/bytecode/codegen/kir/linear_scan_register_allocator.hpp"

#include <vector>

namespace kore {
    namespace kir {
        void LinearScanRegisterAllocator::allocate(
            const std::vector<LiveRange> live_ranges,
            const Function& func
        ) {
            _active_live_ranges.clear();
            _free_registers.clear();

            // Set all registers monotonically used in the function as free
            // registers. Since we are allocating virtual registers to virtual
            // registers, we can just use all those used in the function as
            // allocation will use at most as many registers
            _free_registers.resize(func.max_regs_used());
            std::iota(std::begin(_free_registers), std::end(_free_registers), 0);

            for (const auto& live_range : live_ranges) {
                expire_old_live_ranges(live_range);

                // We don't handle spilling since we are working with virtual registers
                live_range.reg = _free_registers.front();
                _free_registers.pop();
                _active_live_ranges.insert(live_range);
            }

            // Set the new max registers used for the function
            func.set_max_regs_used(_free_registers.top() - 1);

            // Update all registers
            update_registers(live_ranges, func.graph());
        }

        // TODO: Move this function elsewhere
        std::vector<LiveRange> LinearScanRegisterAllocator::get_dead_ranges() {
            std::vector dead_ranges;

            for (const auto& live_range : live_ranges) {
                if (it->is_dead()) {
                    dead_ranges.push_back(live_range);
                }
            }

            return dead_ranges;
        }

        void LinearScanRegisterAllocator::expire_old_live_ranges(const LiveRange& live_range) {
            for (auto& it : _active_live_ranges) {
                // The paper uses >= to compare active live ranges to the
                // current one which may be a mistake. We instead use > since
                // an interval ending at where the current live range starts
                // means that its register should actually be free to use.
                //
                // Consider
                //
                // @3 = @2 + @2
                //
                // If this is the last use of @2, we should be able to use it
                // again for the assignment to @3 so we instead allocate
                // registers as
                //
                // @2 = @2 + @2
                if (it->end > live_range.start) {
                    return;
                }

                it = _active_live_ranges.erase(it);
                _free_registers.push(active_live_range.reg);
            }
        }

        // TODO: Move this to another class/function as allocation should just
        // provide the mapping and not the replacement
        void LinearScanRegisterAllocator::update_registers(
            const std::vector<LiveRange> live_ranges,
            const ControlFlowGraph& cfg
        ) {
            auto iter = this->_cfg.preorder_iterator();

            // TODO: Alternatively save instructions to be modified by live
            // ranges and only update those?
            for (const auto& block : iter) {
                for (const auto& live_range : live_ranges) {
                    if (!is_block_in_range(block, live_range) {
                        continue;
                    }

                    for (const auto& inst : block.instructions) {
                        Reg reg = _register_mapping[*it];

                        // If the original and mapped registers are different, do
                        // the register replacement
                        if (reg == live_range.reg) {
                            continue; 
                        }

                        if (auto ins_type = std::get_if<kir::OneRegister>(&instruction.type)) {
                        // TODO
                        } else if (auto ins_type = std::get_if<kir::TwoRegisters>(&instruction.type)) {
                            switch (opcode) {
                                case Move: {
                                    replace_register(reg, &ins_type->reg2, live_range.reg);
                                    ins_type->reg1 = replace_register(reg, ins_type->reg1);
                                    break;
                                }
                            }
                        } else if (auto ins_type = std::get_if<kir::ThreeRegisters>(&instruction.type)) {
                            switch (opcode) {
                                case AddI32:
                                case AddI64:
                                case AddF32:
                                case AddF64: {
                                    replace_register(reg, &ins_type->reg1, live_range.reg);
                                    replace_register(reg, &ins_type->reg2, live_range.reg);
                                    replace_register(reg, &ins_type->reg3, live_range.reg);
                                    break;
                                }
                            }
                        } else if (auto ins_type = std::get_if<kir::RegisterAndValue>(&instruction.type)) {
                            switch (opcode) {
                                case Cload:
                                case Gload:
                                case LoadBool:
                                case JumpIfNot: {
                                    state.def[ins_type->reg] = true;
                                    update_liveness_state(instruction);
                                    break;
                                }
                            }
                        } else if (auto ins_type = std::get_if<kir::Value>(&instruction.type)) {
                            // NOTE: Only a value is used but no registers
                        } else if (auto ins_type = std::get_if<kir::CallV>(&instruction.type)) {
                        } else if (auto ins_type = std::get_if<kir::ReturnV>(&instruction.type)) {
                            // TODO:
                        }
                    }
                }
            }
        }

        bool is_instruction_in_range(Instruction instruction, LiveRange live_range) {
            return instruction.index >= live_range.start
                && instruction.index <= live_range.end;
        }

        void replace_register(Reg new_reg, Reg* target_reg, Reg expected_reg) {
            // If the target register uses the expected old register, replace
            // it with the new register
            if (*target_reg == expected_reg) {
                *target_reg = new_reg;
            }
        } 
    }
}

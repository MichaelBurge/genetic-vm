#include "vm.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace std;

ExecutionContext::ExecutionContext(const vector<InstructionNode>& _nodes) {
  this->nodes = _nodes;
  for_each(_nodes.begin(), _nodes.end(), [&] (const InstructionNode& node) {
      if (node.instruction != OP_TRIGGER)
        return;
      pending_instructions.insert(node.address);
  });
}

void ExecutionContext::print_nodes() {
  for_each(this->nodes.begin(), this->nodes.end(), [] (const InstructionNode& node) {
      cout << show_instruction_node(node) << endl;
  });
}

void ExecutionContext::print_pending() {
  auto& pending = this->pending_instructions;
  cout << "Pending instructions:" << endl;
  for_each(pending.begin(), pending.end(), [&] (AbsoluteAddress address) {
      auto node = this->get_address(address);
      cout << "Address " << address << ", which is a " << show_instruction_node(node) << endl;
    });
}

InstructionNode& ExecutionContext::get_address(AbsoluteAddress address) {
  return this->nodes[address % this->nodes.size()];
}

bool ExecutionContext::should_execute(const InstructionNode& node) {
  auto ds = dependencies(node);
  return all_of(ds.begin(), ds.end(), [&] (AbsoluteAddress address) {
      return this->get_address(address).active;
    });
}

void ExecutionContext::ensure_dependencies_are_triggered(const InstructionNode& node) {
  auto ds = dependencies(node);
  for_each(ds.begin(), ds.end(), [&] (AbsoluteAddress address) {
      auto node = this->get_address(address);
      if (node.active)
        return;
      this->pending_instructions.insert(address);
  });
}

void ExecutionContext::step() {
    unordered_set<AbsoluteAddress> nodes_to_remove;
    auto& pending = this->pending_instructions;
    for_each(
        pending.begin(),
        pending.end(),
        [&] (const AbsoluteAddress& address) {
          auto node = get_address(address);
          if (this->should_execute(node)) {
            this->execute_node(node);
            nodes_to_remove.insert(address);
          } else {
            this->ensure_dependencies_are_triggered(node);
          }
        });
    for_each(nodes_to_remove.begin(), nodes_to_remove.end(), [&] (AbsoluteAddress address) {
        pending.erase(pending.find(address));
    });
}

 bool ExecutionContext::is_pending(AbsoluteAddress address) {
   return !(this->pending_instructions.find(address) ==
           this->pending_instructions.end());
}

void ExecutionContext::execute_node(const InstructionNode& node) {
  switch (node.instruction) {
  case OP_ADD:
    handle_OP_ADD(node); break;
  case OP_BIND:
    handle_OP_BIND(node); break;
  case OP_BLOCK1:
    handle_OP_BLOCK1(node); break;
  case OP_BLOCK2:
    handle_OP_BLOCK2(node); break;
  case OP_BLOCK3:
    handle_OP_BLOCK3(node); break;
  case OP_BLOCK4:
    handle_OP_BLOCK4(node); break;
  case OP_CONST:
    handle_OP_CONST(node); break;
  case OP_CUT:
    handle_OP_CUT(node); break;
  case OP_DIVIDE:
    handle_OP_DIVIDE(node); break;
  case OP_GEQ:
    handle_OP_GEQ(node); break;
  case OP_GET:
    handle_OP_GET(node); break;
  case OP_IF:
    handle_OP_IF(node); break;
  case OP_LEQ:
    handle_OP_LEQ(node); break;
  case OP_MULTIPLY:
    handle_OP_MULTIPLY(node); break;
  case OP_REGISTER:
    handle_OP_REGISTER(node); break;
  case OP_NOP:
    handle_OP_NOP(node); break;
  case OP_SET:
    handle_OP_SET(node); break;
  case OP_SUBTRACT:
    handle_OP_SUBTRACT(node); break;
  case OP_TRIGGER:
    handle_OP_TRIGGER(node); break;
  default:
    throw logic_error("Unhandled instruction" + show_instruction_node(node));
  }
};

void ExecutionContext::handle_OP_ADD(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BIND(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK1(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK2(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK3(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK4(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_CONST(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_CUT(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_DIVIDE(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_GEQ(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_GET(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_IF(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_LEQ(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_MULTIPLY(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_REGISTER(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_NOP(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_SET(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_SUBTRACT(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_TRIGGER(const InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

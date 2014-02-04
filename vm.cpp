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
  debug = false;
}

void ExecutionContext::print_nodes() {
  print_instruction_nodes(this->nodes);
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
      auto d = this->get_address(address);
      if (d.active)
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
          auto& node = get_address(address);
          if (this->should_execute(node)) {
            if (this->execute_node(node)) {
              node.active = true;
              nodes_to_remove.insert(address);
            }
          } else {
            this->ensure_dependencies_are_triggered(node);
          }
        });
    for_each(nodes_to_remove.begin(), nodes_to_remove.end(), [&] (AbsoluteAddress address) {
        pending.erase(pending.find(address));
    });
}

void ExecutionContext::step_until_done(int max_iterations) {
  while (!(this->pending_instructions.empty()) && max_iterations-- > 0) {
    this->step();
  }
}

bool ExecutionContext::is_pending(AbsoluteAddress address) {
   return !(this->pending_instructions.find(address) ==
           this->pending_instructions.end());
}

bool ExecutionContext::execute_node(InstructionNode& node) {
  if (this->debug)
    cout << "Executing " << show_instruction_node(node) << endl;
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
    return handle_OP_IF(node); break;
  case OP_LEQ:
    handle_OP_LEQ(node); break;
  case OP_MULTIPLY:
    handle_OP_MULTIPLY(node); break;
  case OP_OUTPUT:
    handle_OP_OUTPUT(node); break;
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
  return true;
};

Data ExecutionContext::consume_node(AbsoluteAddress root, RelativeAddress offset) {
  return this->consume_node(translate_relative(root, offset));
}

Data ExecutionContext::consume_node(AbsoluteAddress address) {
  auto& node = this->get_address(address);
  return this->consume_node(node);
}

Data ExecutionContext::consume_node(InstructionNode& node) {
  node.active = false;
  return node.output;
}

void ExecutionContext::handle_OP_ADD(InstructionNode& node) {
  auto d1 = consume_node(node.address, node.input.binop.i1);
  auto d2 = consume_node(node.address, node.input.binop.i2);
  node.output = d1 + d2;
}

void ExecutionContext::handle_OP_BIND(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK1(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK2(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK3(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_BLOCK4(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_CONST(InstructionNode& node) {
  node.output = node.input.data;
}

void ExecutionContext::handle_OP_CUT(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_DIVIDE(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_GEQ(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_GET(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

bool ExecutionContext::handle_OP_IF(InstructionNode& node) {
  int cond, d1, d2;
  switch (node.extra_state) {
  case 0:
    cond = this->consume_node(node.address, node.input.triop.i1);
    node.extra_state = (cond % 2) ? 1 : 2;
    return false;
  case 1:
    d1 = this->consume_node(node.address, node.input.triop.i2);
    node.extra_state = 0;
    node.output = d1;
    return true;
  case 2:
    d2 = this->consume_node(node.address, node.input.triop.i3);
    node.extra_state = 0;
    node.output = d2;
    return true;
  }
  throw logic_error("OP_IF in invalid state");
}

void ExecutionContext::handle_OP_LEQ(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_MULTIPLY(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_OUTPUT(InstructionNode& node) {
  auto data = this->consume_node(node.address, node.input.unop.i);
  this->output_data.push_back(data);
}

void ExecutionContext::handle_OP_REGISTER(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_NOP(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_SET(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_SUBTRACT(InstructionNode&) {
  throw logic_error("Unimplemented instruction");
}

void ExecutionContext::handle_OP_TRIGGER(InstructionNode&) {
}

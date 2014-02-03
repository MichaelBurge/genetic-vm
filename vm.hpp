#pragma once

#include "ast.hpp"
#include <iostream>
#include <unordered_set>
using namespace std;

struct ExecutionContext {
  vector<Data> input_data;
  vector<Data> output_data;
  vector<InstructionNode> nodes;
  unordered_set<AbsoluteAddress> pending_instructions;
  InstructionNode& get_address(AbsoluteAddress);
  bool is_pending(AbsoluteAddress);

  void print_nodes();
  void print_pending();
  void step();
  void step_until_done(int max_iterations);
  ExecutionContext(const vector<InstructionNode>&);
private:
  void ensure_dependencies_are_triggered(const InstructionNode&);
  bool should_execute(const InstructionNode&);
  Data consume_node(AbsoluteAddress);
  Data consume_node(AbsoluteAddress, RelativeAddress);
  Data consume_node(InstructionNode&);
  void execute_node(InstructionNode&);
  void handle_OP_ADD(InstructionNode&);
  void handle_OP_BIND(InstructionNode&);
  void handle_OP_BLOCK1(InstructionNode&);
  void handle_OP_BLOCK2(InstructionNode&);
  void handle_OP_BLOCK3(InstructionNode&);
  void handle_OP_BLOCK4(InstructionNode&);
  void handle_OP_CONST(InstructionNode&);
  void handle_OP_CUT(InstructionNode&);
  void handle_OP_DIVIDE(InstructionNode&);
  void handle_OP_GEQ(InstructionNode&);
  void handle_OP_GET(InstructionNode&);
  void handle_OP_IF(InstructionNode&);
  void handle_OP_LEQ(InstructionNode&);
  void handle_OP_MULTIPLY(InstructionNode&);
  void handle_OP_OUTPUT(InstructionNode&);
  void handle_OP_REGISTER(InstructionNode&);
  void handle_OP_NOP(InstructionNode&);
  void handle_OP_SET(InstructionNode&);
  void handle_OP_SUBTRACT(InstructionNode&);
  void handle_OP_TRIGGER(InstructionNode&);
};

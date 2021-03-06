#pragma once

#include "ast.hpp"
#include <iostream>
#include <unordered_set>
using namespace std;

const int MAX_REGISTERS = 10;

struct ExecutionContext {
  bool debug;
  vector<Data> input_data;
  vector<Data> output_data;
  vector<Data> registers;
  vector<InstructionNode> nodes;
  unordered_set<AbsoluteAddress> pending_instructions;
  InstructionNode& get_address(AbsoluteAddress);
  bool is_pending(AbsoluteAddress);

  void print_nodes();
  void print_pending();
  void print_registers();
  void step();
  void step_until_done(int max_iterations);
  ExecutionContext(const vector<InstructionNode>&);
private:
  Data consume_node(AbsoluteAddress);
  Data consume_node(AbsoluteAddress, RelativeAddress);
  Data consume_node(InstructionNode&);
  void ensure_dependencies_are_triggered(const InstructionNode&);
  bool execute_node(InstructionNode&); // Returns whether should delist node
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
  void handle_OP_GET_BYTE(InstructionNode&);
  void handle_OP_GET_REGISTER(InstructionNode&);
  bool handle_OP_IF(InstructionNode&);
  void handle_OP_LEQ(InstructionNode&);
  void handle_OP_MULTIPLY(InstructionNode&);
  void handle_OP_OUTPUT(InstructionNode&);
  void handle_OP_NOP(InstructionNode&);
  void handle_OP_SET_BYTE(InstructionNode&);
  void handle_OP_SET_REGISTER(InstructionNode&);
  void handle_OP_SUBTRACT(InstructionNode&);
  void handle_OP_TRIGGER(InstructionNode&);
  bool should_execute(const InstructionNode&);
  uint8_t translate_register(int8_t);
};

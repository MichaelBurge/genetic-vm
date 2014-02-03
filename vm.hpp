#pragma once

#include "ast.hpp"
#include <unordered_set>
using namespace std;

struct ExecutionContext {
  vector<InstructionNode> nodes;
  unordered_set<AbsoluteAddress> pending_instructions;
  InstructionNode& get_address(AbsoluteAddress);
  void execute_node(const InstructionNode&);
  bool is_pending(AbsoluteAddress);
  void print_nodes();
  void print_pending();
  void step();
  ExecutionContext(const vector<InstructionNode>&);
private:
  void ensure_dependencies_are_triggered(const InstructionNode&);
  bool should_execute(const InstructionNode&);
  void handle_OP_ADD(const InstructionNode&);
  void handle_OP_BIND(const InstructionNode&);
  void handle_OP_BLOCK1(const InstructionNode&);
  void handle_OP_BLOCK2(const InstructionNode&);
  void handle_OP_BLOCK3(const InstructionNode&);
  void handle_OP_BLOCK4(const InstructionNode&);
  void handle_OP_CONST(const InstructionNode&);
  void handle_OP_CUT(const InstructionNode&);
  void handle_OP_DIVIDE(const InstructionNode&);
  void handle_OP_GEQ(const InstructionNode&);
  void handle_OP_GET(const InstructionNode&);
  void handle_OP_IF(const InstructionNode&);
  void handle_OP_LEQ(const InstructionNode&);
  void handle_OP_MULTIPLY(const InstructionNode&);
  void handle_OP_REGISTER(const InstructionNode&);
  void handle_OP_NOP(const InstructionNode&);
  void handle_OP_SET(const InstructionNode&);
  void handle_OP_SUBTRACT(const InstructionNode&);
  void handle_OP_TRIGGER(const InstructionNode&);
};

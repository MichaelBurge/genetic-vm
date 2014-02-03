#pragma once

#include "ast.hpp"

struct ExecutionContext {
  vector<InstructionNode> nodes;
  void dispatch_node(const InstructionNode&);
  void step();
  ExecutionContext(const vector<InstructionNode>& _nodes) : nodes(_nodes) { }
private:
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

#pragma once

#include <stdint.h>
#include <vector>
using namespace std;

typedef int8_t Address;

enum Instruction {
    OP_ADD,
    OP_BIND,
    OP_BLOCK1,
    OP_BLOCK2,
    OP_BLOCK3,
    OP_BLOCK4,
    OP_CALL,
    OP_CONST,
    OP_CUT,
    OP_DIVIDE,
    OP_GEQ,
    OP_GET,
    OP_IF,
    OP_INPUT,
    OP_LEQ,
    OP_MULTIPLY,
    OP_OUTPUT,
    OP_REGISTER,
    OP_NOP,
    OP_SET,
    OP_SUBTRACT,
    OP_TRIGGER,
};

struct InstructionNode {
    Instruction type;
    union {
        int8_t data;
        struct { Address i; } unop;
        struct { Address i1, i2; } binop;
        struct { Address i1, i2, i3; } triop;
    } input;
};

extern vector<InstructionNode> lift_bytes_to_graph(const vector<int8_t>&);

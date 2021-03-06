#pragma once

#include <map>
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;

typedef int8_t RelativeAddress;
typedef int8_t Data;
typedef int8_t Ring;
typedef uint16_t AbsoluteAddress;

enum Instruction {
    OP_ADD,
    OP_BIND,
    OP_BLOCK1,
    OP_BLOCK2,
    OP_BLOCK3,
    OP_BLOCK4,
    OP_CONST,
    OP_CUT,
    OP_DIVIDE,
    OP_GEQ,
    OP_GET_BYTE,
    OP_GET_REGISTER,
    OP_IF,
    OP_LEQ,
    OP_MULTIPLY,
    OP_OUTPUT,
    OP_NOP,
    OP_SET_BYTE,
    OP_SET_REGISTER,
    OP_SUBTRACT,
    OP_TRIGGER,
};

enum InstructionType {
  IT_NOINPUT,
  IT_DATA,
  IT_UNOP,
  IT_BINOP,
  IT_TRIOP,
  IT_QUADOP,
  IT_RING_UNOP,
  IT_RING_BINOP,
};

struct InstructionNode {
    AbsoluteAddress address;
    Instruction instruction;
    int  extra_state;
    bool active;
    Data output;
    union {
        Data data;
        struct { RelativeAddress i; } unop;
        struct { RelativeAddress i1, i2; } binop;
        struct { RelativeAddress i1, i2, i3; } triop;
        struct { RelativeAddress i1, i2, i3, i4; } quadop;
        struct { Ring r; RelativeAddress i1; } ring_unop;
        struct { Ring r; RelativeAddress i1, i2; } ring_binop;
    } input;
  InstructionNode() : extra_state(0), active(false), output(0) { }
};

extern map<Instruction, string> instruction_names;
extern map<InstructionType, string> instruction_type_names;

extern Instruction instruction_from_bytes(int8_t);
extern string show_instruction_node(const InstructionNode&);
extern void print_instruction_nodes(const vector<InstructionNode>&);
extern int num_inputs_for_instruction_type(InstructionType);
extern InstructionType instruction_type(Instruction);
extern vector<InstructionNode> lift_bytes_to_graph(const vector<int8_t>&);
extern vector<AbsoluteAddress> dependencies(const InstructionNode&);
extern AbsoluteAddress translate_relative(const InstructionNode&, RelativeAddress);
extern AbsoluteAddress translate_relative(AbsoluteAddress, RelativeAddress);

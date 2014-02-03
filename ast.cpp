#include "ast.hpp"
#include <algorithm>
#include <boost/assign/list_of.hpp>
#include <stdexcept>
#include <vector>
using namespace std;

void consume_input(InstructionNode& node, InstructionType type, int input_no, int8_t block);

vector<InstructionNode> lift_bytes_to_graph(const vector<int8_t>& bytes) {
  vector<InstructionNode> ret;
  AbsoluteAddress next_address = 0;
  InstructionNode current;
  int num_inputs = 0;
  auto handle_instruction_input = [&] (int8_t block) {
    auto type = instruction_type(current.instruction);
    consume_input(current, type, num_inputs--, block);
  };
  auto finalize_instruction = [&] () {
    current.address = next_address++;
    ret.push_back(current);
  };
  for_each(bytes.begin(), bytes.end(), [&] (const int8_t block) {
      auto handle_new_instruction = [&] () {
        auto instruction = instruction_from_bytes(block);
        auto type = instruction_type(instruction);
        current.instruction = instruction;
        num_inputs = num_inputs_for_instruction_type(type);
      };

      if (num_inputs == 0) {
        handle_new_instruction();
      } else {
        handle_instruction_input(block);
      }
      if (num_inputs == 0)
        finalize_instruction();
    });
  return ret;
}

Instruction instruction_from_bytes(int8_t block) {
  return static_cast<Instruction>(block);
}
                                                 

void consume_input(InstructionNode& node, InstructionType type, int input_no, int8_t block) {
  auto consume_noinput = [] () {
    throw logic_error("Bad # of inputs");
  };
  auto consume_data = [&] () {
    switch (input_no) {
    case 1:
      node.input.data = block; break;
    default:
      throw logic_error("Bad # of inputs");
    }
  };
  auto consume_unop = [&] () {
    switch (input_no) {
    case 1:
      node.input.unop.i = block; break;
    default:
      throw logic_error("Bad # of inputs");
    }
  };
  auto consume_binop = [&] () {
    switch (input_no) {
    case 2:
      node.input.binop.i1 = block; break;
    case 1:
      node.input.binop.i2 = block; break;
    default:
      throw logic_error("Bad # of inputs");
    }
  };
  auto consume_triop = [&] () {
    switch (input_no) {
    case 3:
      node.input.triop.i1 = block; break;
    case 2:
      node.input.triop.i2 = block; break;
    case 1:
      node.input.triop.i3 = block; break;
    default:
      throw logic_error("Bad # of inputs");
    }
  };
  auto consume_quadop = [&] () {
    switch (input_no) {
    case 4:
      node.input.quadop.i1 = block; break;
    case 3:
      node.input.quadop.i2 = block; break;
    case 2:
      node.input.quadop.i3 = block; break;
    case 1:
      node.input.quadop.i4 = block; break;
    default:
      throw logic_error("Bad # of inputs");
    }
  };
  auto consume_ring_unop = [&] () {
    switch (input_no) {
    case 2:
      node.input.ring_unop.r = block; break;
    case 1:
      node.input.ring_unop.i1 = block; break;
    default:
      throw logic_error("Bad # of inputs");
    }
  };
  auto consume_ring_binop = [&] () {
    switch (input_no) {
    case 3:
      node.input.ring_binop.r = block; break;
    case 2:
      node.input.ring_binop.i1 = block; break;
    case 1:
      node.input.ring_binop.i2 = block; break;
    default:
      throw logic_error("Bad # of inputs");
    }
  };
  switch (type) {
  case IT_NOINPUT:
    consume_noinput(); break;
  case IT_DATA:
    consume_data(); break;
  case IT_UNOP:
    consume_unop(); break;
  case IT_BINOP:
    consume_binop(); break;
  case IT_TRIOP:
    consume_triop(); break;
  case IT_QUADOP:
    consume_quadop(); break;
  case IT_RING_UNOP:
    consume_ring_unop(); break;
  case IT_RING_BINOP:
    consume_ring_binop(); break;
  default:
    throw logic_error("Unknown instruction type");
  }
}

std::map<Instruction, std::string> instruction_names = boost::assign::map_list_of
  (OP_ADD,      "OP_ADD")
  (OP_BIND,     "OP_BIND")
  (OP_BLOCK1,   "OP_BLOCK1")
  (OP_BLOCK2,   "OP_BLOCK2")
  (OP_BLOCK3,   "OP_BLOCK3")
  (OP_BLOCK4,   "OP_BLOCK4")
  (OP_CONST,    "OP_CONST")
  (OP_CUT,      "OP_CUT")
  (OP_DIVIDE,   "OP_DIVIDE")
  (OP_GEQ,      "OP_GEQ")
  (OP_GET,      "OP_GET")
  (OP_IF,       "OP_IF")
  (OP_LEQ,      "OP_LEQ")
  (OP_MULTIPLY, "OP_MULTIPLY")
  (OP_OUTPUT,   "OP_OUTPUT")
  (OP_REGISTER, "OP_REGISTER")
  (OP_NOP,      "OP_NOP")
  (OP_SET,      "OP_SET")
  (OP_SUBTRACT, "OP_SUBTRACT")
  (OP_TRIGGER,  "OP_TRIGGER")
  ;

map<InstructionType, string> instruction_type_names = boost::assign::map_list_of
  (IT_NOINPUT,    "IT_NOINPUT")
  (IT_DATA,       "IT_DATA")
  (IT_UNOP,       "IT_UNOP")
  (IT_BINOP,      "IT_BINOP")
  (IT_TRIOP,      "IT_TRIOP")
  (IT_QUADOP,     "IT_QUADOP")
  (IT_RING_UNOP,  "IT_RING_UNOP")
  (IT_RING_BINOP, "IT_RING_BINOP")
  ;

InstructionType instruction_type(Instruction instruction) {
  switch (instruction) {
  case OP_NOP:
  case OP_CUT:
    return IT_NOINPUT;

  case OP_CONST:
    return IT_DATA;

  case OP_BLOCK1:
  case OP_OUTPUT:
  case OP_REGISTER:
  case OP_TRIGGER:
    return IT_UNOP;

  case OP_ADD:
  case OP_BIND:
  case OP_BLOCK2:
  case OP_DIVIDE:
  case OP_GEQ:
  case OP_LEQ:
  case OP_MULTIPLY:
  case OP_SUBTRACT:
    return IT_BINOP;

  case OP_BLOCK3:
  case OP_IF:
    return IT_TRIOP;

  case OP_BLOCK4:
    return IT_QUADOP;

  case OP_GET:
    return IT_RING_UNOP;

  case OP_SET:
    return IT_RING_BINOP;

  default:
    throw logic_error("Unimplemented instruction");
  }
}

int num_inputs_for_instruction_type(InstructionType type) {
  switch (type) {
  case IT_NOINPUT:
    return 0;
  case IT_DATA:
  case IT_UNOP:
    return 1;
  case IT_BINOP:
  case IT_RING_UNOP:
    return 2;
  case IT_RING_BINOP:
  case IT_TRIOP:
    return 3;
  case IT_QUADOP:
    return 4;
  default:
    throw logic_error("Invalid instruction");
  }
}

string show_instruction_node(const InstructionNode& node) {
  auto instruction = node.instruction;
  auto type = instruction_type(instruction);
  string instruction_name = instruction_names[instruction];
  string instruction_type_name = instruction_type_names[type];
  string base_output =
    instruction_name + " (" +
    "Active=" + string(node.active ? "Y" : "N") + "," + 
    "Type=" + instruction_type_name
    ;
  string extra_output;
  switch (type) {
  case IT_NOINPUT:
    extra_output = ""; break;
  case IT_DATA:
    extra_output = ",Data=" + to_string(node.input.data); break;
  case IT_UNOP:
    extra_output = ",IAddr=" + to_string(node.input.unop.i); break;
  case IT_BINOP:
    extra_output  = ",IAddr1=" + to_string(node.input.binop.i1);
    extra_output += ",IAddr2=" + to_string(node.input.binop.i2);
    break;
  case IT_TRIOP:
    extra_output  = ",IAddr1=" + to_string(node.input.triop.i1);
    extra_output += ",IAddr2=" + to_string(node.input.triop.i2);
    extra_output += ",IAddr3=" + to_string(node.input.triop.i3);
    break;
  case IT_QUADOP:
    extra_output  = ",IAddr1=" + to_string(node.input.quadop.i1);
    extra_output += ",IAddr2=" + to_string(node.input.quadop.i2);
    extra_output += ",IAddr3=" + to_string(node.input.quadop.i3);
    extra_output += ",IAddr4=" + to_string(node.input.quadop.i4);
    break;
  case IT_RING_UNOP:
    extra_output  = ",Ring=" + to_string(node.input.ring_unop.r);
    extra_output += ",IAddr=" + to_string(node.input.ring_unop.i1);
    break;
  case IT_RING_BINOP:
    extra_output  = ",Ring=" + to_string(node.input.ring_binop.r);
    extra_output += ",IAddr1=" + to_string(node.input.ring_binop.i1);
    extra_output += ",IAddr2=" + to_string(node.input.ring_binop.i2);
    break;
  default:
    throw logic_error("Unknown instruction type");
  }
  return base_output + extra_output + ")";
}

AbsoluteAddress translate_relative(const InstructionNode& node, RelativeAddress offset) {
  return translate_relative(node.address, offset);
}

AbsoluteAddress translate_relative(AbsoluteAddress base, RelativeAddress offset) {
  return static_cast<AbsoluteAddress>(base + offset);
}

vector<AbsoluteAddress> dependencies(const InstructionNode& node) {
  auto ret = vector<AbsoluteAddress>();
  auto type = instruction_type(node.instruction);
  switch (type) {
  case IT_NOINPUT:
    break;
  case IT_DATA:
    break;
  case IT_UNOP:
    ret.push_back(translate_relative(node, node.input.unop.i));
    break;
  case IT_BINOP:
    ret.push_back(translate_relative(node, node.input.binop.i1));
    ret.push_back(translate_relative(node, node.input.binop.i2));
    break;
  case IT_TRIOP:
    ret.push_back(translate_relative(node, node.input.triop.i1));
    ret.push_back(translate_relative(node, node.input.triop.i2));
    ret.push_back(translate_relative(node, node.input.triop.i3));
    break;
  case IT_QUADOP:
    ret.push_back(translate_relative(node, node.input.quadop.i1));
    ret.push_back(translate_relative(node, node.input.quadop.i2));
    ret.push_back(translate_relative(node, node.input.quadop.i3));
    ret.push_back(translate_relative(node, node.input.quadop.i4));
    break;
  case IT_RING_UNOP:
    break;
  case IT_RING_BINOP:
    ret.push_back(translate_relative(node, node.input.ring_binop.i2));
    break;
  default:
    throw logic_error("Unknown instruction type");
  }
  return ret;
}

void print_instruction_nodes(const vector<InstructionNode>& nodes) {
  for_each(nodes.begin(), nodes.end(), [] (const InstructionNode& node) {
      cout << show_instruction_node(node) << endl;
  });
}

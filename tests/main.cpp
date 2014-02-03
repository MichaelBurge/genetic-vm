#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Instructions

#include "../ast.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
using namespace std;

// Example Program:
// for (i = 0; i <= length-1; i++) {
// copy self[i] to target[i]
// }
// cut

vector<int8_t> sample_program() {
  vector<int8_t> ret{
    // i = 0
    OP_BIND,  1, 0,
    OP_CONST, 0,
    // value of i <= length
    OP_LEQ,      1, 2,
    OP_REGISTER, 0,
    OP_CONST,    16, // length of program in instructions, minus 1
    // i++
    OP_SET,      0, 1, 2,
    OP_REGISTER, 0,
    OP_ADD,      -1, 1,
    OP_CONST,    1,
    // copy self[i] to target[i]
    OP_SET, 1, 2, 1,
    OP_GET, 0, 1,
    OP_REGISTER, 0,
    // for
    OP_BLOCK2, -12, 1,
    OP_IF,     -11, 1, 2,
    OP_BLOCK3, -5, -9, -1,
    // cut
    OP_CUT,
    // Main program
    OP_TRIGGER, -4,
      };
  return ret;
}

BOOST_AUTO_TEST_CASE( print_sample_program) {
  vector<InstructionNode> nodes = lift_bytes_to_graph(sample_program());
  BOOST_TEST_MESSAGE("Printing sample program");
  for_each(nodes.begin(), nodes.end(), [] (const InstructionNode& node) {
      BOOST_TEST_MESSAGE(show_instruction_node(node));
  });
}

BOOST_AUTO_TEST_CASE( check_sample_program ) {
  vector<InstructionNode> nodes = lift_bytes_to_graph(sample_program());
  BOOST_CHECK_EQUAL(nodes.size(), 17);
}

BOOST_AUTO_TEST_CASE( num_instructions) {
  auto num_instructions = 19;
  auto num_instruction_types = 8;
  BOOST_CHECK_EQUAL(instruction_names.size(), num_instructions);
  BOOST_CHECK_EQUAL(instruction_type_names.size(), num_instruction_types);

  auto check_instruction = [] (Instruction instruction) {
    BOOST_TEST_MESSAGE("Checking instruction " + instruction_names[instruction]);
    auto type = instruction_type(instruction);
    num_inputs_for_instruction_type(type);
  };
  for (int8_t i = 0; i != num_instructions-1; i++) {
    check_instruction(instruction_from_bytes(i));
  }
}

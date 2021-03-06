#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Instructions

#include "../ast.hpp"
#include "../vm.hpp"
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
  const int8_t program_length = 37;
  vector<int8_t> ret{
    // value of i <= length
    OP_LEQ,          1, 2,
    OP_GET_REGISTER, 0,
    OP_CONST,        program_length,
    // i++
    OP_SET_REGISTER, 0, 1,
    OP_ADD,          1, 2,
    OP_GET_REGISTER, 0,
    OP_CONST,        1,
    // copy self[i] to target[i]
    OP_SET_BYTE,     1, 2, 1,
    OP_GET_BYTE,     0, 1,
    OP_GET_REGISTER, 0,
    // for
    OP_IF,     -10, 1, 2,
    OP_BLOCK3, -4, -8, -1,
    // cut
    OP_CUT,
    // Main program
    OP_TRIGGER, -3,
  };
  BOOST_REQUIRE_EQUAL((int)ret.size(), (int)program_length);
  return ret;
}

BOOST_AUTO_TEST_CASE( sample_addition_program) {
  vector<int8_t> addition_program{
    OP_CONST, 6,
    OP_CONST, 7,
    OP_ADD, -1, -2,
    OP_OUTPUT, -1,
    OP_TRIGGER, -1,
  };
  auto nodes = lift_bytes_to_graph(addition_program);
  auto context = ExecutionContext(nodes);
  context.step();
  context.step();
  context.step();
  context.step();
  BOOST_CHECK_EQUAL(context.output_data.size(), 1);
  BOOST_CHECK_EQUAL(context.output_data[0], 13);
}

BOOST_AUTO_TEST_CASE( print_sample_program) {
  vector<InstructionNode> nodes = lift_bytes_to_graph(sample_program());
  BOOST_TEST_MESSAGE("Printing sample program");
  for_each(nodes.begin(), nodes.end(), [] (const InstructionNode& node) {
      BOOST_TEST_MESSAGE(show_instruction_node(node));
  });
}

BOOST_AUTO_TEST_CASE( updates_pending_correctly) {
  vector<InstructionNode> nodes = lift_bytes_to_graph(sample_program());
  auto context = ExecutionContext(nodes);
  BOOST_CHECK_EQUAL(context.pending_instructions.size(), 1);
  BOOST_CHECK(!context.is_pending(10));
  BOOST_CHECK( context.is_pending(13));
  context.step();
  BOOST_CHECK_EQUAL(context.pending_instructions.size(), 2);
  BOOST_CHECK( context.is_pending(10));
  BOOST_CHECK( context.is_pending(13));
}

BOOST_AUTO_TEST_CASE( check_sample_program ) {
  vector<InstructionNode> nodes = lift_bytes_to_graph(sample_program());
  BOOST_CHECK_EQUAL(nodes.size(), 14);
  auto context = ExecutionContext(nodes);
  context.debug = true;
  context.step();
  context.print_pending();
  context.step();
  context.print_pending();
  context.step();
  context.print_pending();
  context.step();
  context.print_pending();
  context.step();
  context.print_pending();
  context.step();
  context.print_pending();
  context.print_registers();
  context.step();
  context.print_pending();
  context.print_registers();
  context.step();
  context.print_pending();
  context.print_registers();
}

BOOST_AUTO_TEST_CASE( op_if ) {
  vector<int8_t> if_program{
    OP_CONST,    5,
    OP_CONST,    6,
    OP_CONST,    1,
    OP_IF,      -1, -2, -3,
    OP_OUTPUT,  -1,
    OP_TRIGGER, -1,
  };
  vector<InstructionNode> nodes = lift_bytes_to_graph(if_program);
  auto context = ExecutionContext(nodes);
  context.print_nodes();
  context.step_until_done(10);
  BOOST_CHECK_EQUAL(context.output_data.size(), 1);
  BOOST_CHECK_EQUAL((int)context.output_data[0], (int)6);
}

BOOST_AUTO_TEST_CASE( op_if_is_lazy) {
  vector<int8_t> if_program{
    OP_CONST,    5,
    OP_OUTPUT,  -1,
    OP_CONST,    6,
    OP_OUTPUT,  -1,
    OP_CONST,    1,
    OP_IF,      -1, -2, -4,
    OP_TRIGGER, -1,
  };
  vector<InstructionNode> nodes = lift_bytes_to_graph(if_program);
  auto context = ExecutionContext(nodes);
  context.step_until_done(10);
  BOOST_CHECK_EQUAL(context.output_data.size(), 1);
  BOOST_CHECK_EQUAL(context.output_data[0], 6);
}

BOOST_AUTO_TEST_CASE( num_instructions) {
  auto num_instructions = 21;
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

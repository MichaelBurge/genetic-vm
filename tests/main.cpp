#include "../ast.hpp"
#include "../test.hpp"
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
    OP_CONST,    17, // length of program in instructions, minus 1
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
    OP_TRIGGER,
      };
  return ret;
}

void test_sample_program() {
  vector<InstructionNode> nodes = lift_bytes_to_graph(sample_program());
  assert_eq(nodes.size(), 18, "# of instructions wrong");
}

int main() {
  test_sample_program();
  return 0;
}

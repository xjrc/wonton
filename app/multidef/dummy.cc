/*
This file is part of the Ristra Wonton project.
Please see the license file at the root of this repository, or at:
    https://github.com/laristra/wonton/blob/master/LICENSE
*/

// Dummy subroutine which also includes same headers as main.cc to
// check if we end up with multiply defined symbols because of
// function definitions in included in header files (class variables
// are always inlined)

#include <iostream>
#include "wonton_includes_to_check.h"

int dummy(int i) {
  std::cout << "Test function to see if we get multiply defined symbols\n";
  return i;
}

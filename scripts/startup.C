#include "TTree.h"

int startup() {
  TTree::SetMaxTreeSize( 1500000000000LL ); // 1.5 TB
  return 0;
}

namespace {
  static int i = startup();
}

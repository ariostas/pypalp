#pragma once

extern "C" {
#include "Global.h"
#include "LG.h"
}

// We we reexport some types used in PALP and wrap others into structs to make
// them more convenient.

struct PairMatStruct {
  PairMat data;
};


// ============================================================================
// Takes a Var binary file name, and extracts/returns Var array from it.
// 
// See class ZtdbVarWr.h for format of file
// ============================================================================

#include <vector>

#include "ZtdbVar.h"

#pragma once
class ZtdbVarRd {
public:
	static void read ( string fileName, vector<ZtdbVar> &vars );
};


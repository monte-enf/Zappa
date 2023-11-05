
// ============================================================================
// Takes a file name and Var array and writes a binary file
//
// Format of file
// 
// V1.0 - Aligns to 32 bit/4 byte words
// 
//   Column									Addr	Comment
// ==============================================================================
//  +33222222_22221111_111111
//  +10987654_32109876_54321098_76543210
//  +VVVVVVVV_VVVVVVVV.vvvvvvvv_vvvvvvvv 	0x0000	Version (8-bit major.8-bit minor)
//  +########_########_########_########    0x0004	Number of vars (Much too large...)
//  +BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB	0x0008	Base offset to name strings
//  +ttSSSSSS_oooooooo_oooooooo_oooooooo	0x000C+	Var Information - 1 for each variable
//  ...
//  +NNNNNNNN_NNNNNNNN_NNNNNNNN_NNNNNNNN	Name strings
//  ...
// 
//  Where:
//      V = Major version
//      v = minor version
//      # = Number of variables
//		B = Base offset to name strings buffer
//      t = Type (See ZtdbCommon.h for enum of types)
//      S = Size (64 represented by 0).  V1 limited to max signals of 64 bits
//      o = Offset in name buffer to name.   Name found at <base offset> + offset.  
//      N = Name in char* format.  Names are 0 terminated.
//
// ============================================================================
// Commentary:
// 
// V,v - god forbit if run out of version options...  Supports 64k Major and minor variants
// #   - Arguably don't need both numVars and offset, but no harm, so leaving it be.
// B   - This field is much larger than required, but lines things up nicely.
// t   - Allow 4 types.  Currently only 1.  Maybe 4 state variables in future?
// S   - Currently limited to 64 bits
// 
// o   - Offset limited to 2^24, or 16Mbyte.  
//       Presuming each name 256 characters (which is very generous)
//       allows for 64K variables.  
//       That seems reasonable... 
// 
// ============================================================================
// To dump file from unix command line:
// 
//  > hexdump -v -e '"%10_ax |" 1/4 "%08x" "|\n" ' <binaryVarFile>
// 
// ============================================================================

#pragma once

#include "ZtdbVar.h"

class ZtdbVarWr {
public:
	static void write ( string fileName, vector<ZtdbVar> vars );
};


#pragma once

#include <iostream>		// Std::ofstream
#include <fstream>      // std::ifstream
#include <string>
#include <map>
#include <vector>

using namespace std;

// ============================================================================
// Types, Constants, Enums, Structs and Unions common to Ztdb library
// ============================================================================
// Types

typedef uint32_t ZtdbSize;
typedef uint64_t ZtdbTime;

// ZtdbValue is simular to a 64 unsigned int, except for special values - see constants below
typedef uint64_t ZtdbValue;


// ============================================================================
// Constants

// Special values of ZtdbValue
constexpr auto ZTDB_MAX = 0xFFFFFFFFFFFFFFFFULL;
constexpr auto ZTDB_UNKOWN = 0xFFFFFFFFFFFFFFFEULL;
constexpr auto ZTDB_RANGE = 0xFFFFFFFFFFFFFFFDULL;  // Value doesn't fit


// ============================================================================
// Enums

enum VarTypes {
	VarTypeDefault   = 0,
	VarTypeReserved1 = 1,
	VarTypeReserved2 = 2,
	VarTypeReserved3 = 3
};


// ============================================================================
// ============================================================================
// Structs/Unions used to write/read binary files
// ============================================================================
// ============================================================================
// Version

typedef struct {					// defines how version stored in DB
	uint16_t minor : 16;
	uint16_t major : 16;
} versionStruct;

union versionUnion {				// Used to read and write version from a file
	char          b[4];
	versionStruct s;
};

// ========================================================================
// Number

union numberUnion {				// Used to read and write version from a file
	char        b[4];
	uint32_t    n;
};

// ========================================================================
// Offset

union offsetUnion {				// Used to read and write version from a file
	char        b[4];
	uint32_t    o;
};

// ========================================================================
// Var 

typedef struct {
	unsigned int o : 24;			// offset - 24 bits wide
	unsigned int s : 6;				// size   - 6 bits wide
	unsigned int t : 2;				// type   - 2 bits wide
} varStruct;

union varUnion {
	char		b[4];
	varStruct   v;
};




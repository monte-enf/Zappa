#include "ZtdbVarRd.h"

void ZtdbVarRd::read ( string fileName, vector<ZtdbVar> &vars ) {

	ifstream in ( fileName, ios_base::in | ios_base::binary );

	if ( !in ) {
		cout << "ZtdbVarRd::read ERROR: Could not open fileName=" << fileName << "\n";
		return;
	}

	vars.clear ( );

	// ========================================================================
	// Version

	versionUnion v;					// Version 1.0
	in.read ( v.b, sizeof ( v.b ) );

	if ( ( v.s.major != 1 ) ||
	 	 ( v.s.minor != 0 ) ) {
		cout << "ZtdbVarRd::read ERROR: Version of file=" << fileName
			 << " is " << v.s.major << "." << v.s.minor
			 << " which is != 1.0 - not supported\n";
		return;
	}

	// ========================================================================
	// Number of vars

	numberUnion n;
	in.read ( n.b, sizeof ( n.b ) );

	uint32_t numVars = n.n;

	cout << "ZtdbVarRd::read filename=" << fileName << " numVars = " << numVars << "\n"; // debug

	// ========================================================================
	// Offset

	offsetUnion o;
	in.read ( o.b, sizeof ( o.b ) );

	uint32_t offsetStart = o.o;

	// ========================================================================
	// Var data

	vector<uint32_t> nameOffset;

	for ( uint32_t ith = 0; ith < numVars; ith++ ) {
		varUnion v;
		in.read ( v.b, sizeof ( v.b ) );

		uint32_t size = v.v.s;
		if ( size == 0 ) {			// Special case - 0 means max size of 64
			size = 64;
		}

		// Currently only 1 type, so v.v.t (VarType) is ignored.
		vars.push_back ( ZtdbVar ( "tbd", size ) );
		nameOffset.push_back ( v.v.o );		// Save offset for later
	}

	// ========================================================================
	// Var name

	for ( uint32_t ith = 0; ith < numVars; ith++ ) {
		in.seekg ( nameOffset[ith] );
		getline ( in, vars[ith].mName, char ( 0 ) );
	}


	// ========================================================================

	in.close ( );

}

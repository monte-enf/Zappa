#include <iostream>
#include <fstream>

#include "ZtdbVarWr.h"

void ZtdbVarWr::write ( string fileName, vector<ZtdbVar> vars ) {

	ofstream out ( fileName, ios_base::out | ios_base::binary ); 

	if ( !out ) {
		cout << "ZtdbVarWr::write ERROR: Could not open fileName=" << fileName << "\n";
		return;
	}

	// ========================================================================
	// Version

	versionUnion v;					// Version 1.0
	v.s.major = 1;
	v.s.minor = 0;
	out.write ( v.b, sizeof ( v.b ) );

	// ========================================================================
	// Number of vars

	numberUnion n;
	n.n = vars.size ( );
	out.write ( n.b, sizeof ( n.b ) );

	// ========================================================================
	// Offset

	uint32_t offsetStart = uint32_t ( 0xC +						// Points to word after where this is stored
						              ( vars.size ( ) * sizeof ( varUnion ) ) );

	offsetUnion o;
	o.o = offsetStart;
	out.write ( o.b, sizeof ( o.b ) );

	// ========================================================================
	// Var data

	uint32_t offsetCur = offsetStart;
	for ( auto &aVar : vars ) {
		varUnion v;
		v.v.t = VarTypeDefault;			// Only 1 type currently supported
		v.v.s = aVar.getSize ( );		// Width of variable in bits.  64 written as 0.
		v.v.o = offsetCur;				// Where name will go
		out.write ( v.b, sizeof ( v.b ) );

		offsetCur += uint32_t ( aVar.mName.size ( ) + 1 ); 
	}

	// ========================================================================
	// Var name

	if ( out.tellp ( ) != offsetStart ) {
		cout << "ZtdbVarWr::write ERROR: expected start of names @=" << hex
 			 << " offsetStart=" << offsetStart
			 << " but tellp=" << out.tellp ( ) << " (this is super bad)\n";
	}

	for ( auto &aVar : vars ) {
		out.write ( aVar.mName.c_str ( ), aVar.mName.size ( ) + 1 );
	}

	if ( out.tellp ( ) != offsetCur ) {
		cout << "ZtdbVarWr::write ERROR: expected end of names @=" << hex
			<< " offsetCur=" << offsetCur
			<< " but tellp=" << out.tellp ( ) << " (this is super bad)\n";
	}

	// ========================================================================

	out.close ( );

}

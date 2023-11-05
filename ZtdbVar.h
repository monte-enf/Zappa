#pragma once

#include "Ztdb.h"

// ============================================================================
// ZtdbVar represents 1 variable
// It includes time/Value data;

class ZtdbVar {
	public:
	// ========================================================================
	// Members

	ZtdbSize				mSize;								// Width of variable in bits
	string					mName;								// Name of var
	ZtdbValue				mMax;								// Largest value, inclusive

	multimap<ZtdbTime, ZtdbValue> mTimeValue;					// Storage for time/values
	multimap<ZtdbTime, ZtdbValue>::iterator mTvIth;				// Iterator used by getFirst and getNext
	

	// ========================================================================
	// Methods
	ZtdbVar           ( string name, uint32_t size );

	void     add      ( ZtdbTime time, ZtdbValue value );		// Add T/V to storage
	bool     getFirst ( ZtdbTime &time, ZtdbValue &value );		// TRUE and first T/V if exists (else return false)
	bool     getNext  ( ZtdbTime &time, ZtdbValue &value );		// TRUE and next T/V if exists (else returns false)
	bool     getAt    ( ZtdbTime target, ZtdbTime  &time, ZtdbValue &value ); // TRUE and next T/V at or after specified time. 

	uint32_t getSize  ( );
	uint32_t getNum   ( );										// Returns number of Time/Value pairs

	// Methods to help dump
	float    timePrecise    ( ZtdbTime time, int precision );	// Converts raw time into time with precision digits to right of decimal piont
	uint32_t getValueSize   ( bool hex );
	string   getValueString ( ZtdbValue value, bool hex );		
	void     dump           ( string prefix, uint32_t precision, bool hex );  // Precision=digits to right of time  hex=data format
};



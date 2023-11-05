#include <iostream>     // cout, endl
#include <iomanip>      // setw

#include "ZtdbVar.h"


ZtdbVar::ZtdbVar ( string name, uint32_t size )
	: mSize ( size ),
	  mName ( name ) {

	if ( mSize > 64 ) {
		// FATAL! how do that?
		cout << "ZtdbVar::ZtdbVar ERROR:"
			 << " name=" << name
			 << " size=" << size
			 << "  > 64 - truncated to 64\n\n";
		mSize = 64;
	}

	if ( mSize == 0 ) {
		cout << "ZtdbVar::ZtdbVar ERROR:"
			<< " name=" << name
			<< " size=" << size
			<< "  == 0 - set to 1\n\n";
		mSize = 1;
	}

	mMax = ZTDB_MAX >> ( 64 - mSize );

	// DEBUG cout << "ZtdbVar::ZtdbVar: ZTDB_MAX=" << hex << ZTDB_MAX << "\n";

}


void ZtdbVar::add ( ZtdbTime time, ZtdbValue value ) {
	if ( ( value > mMax         ) &&
 		 ( value != ZTDB_UNKOWN ) ) {
		value = ZTDB_RANGE;
	}

	mTimeValue.insert ( pair<ZtdbTime, ZtdbValue> ( time, value ) );
}


bool ZtdbVar::getFirst ( ZtdbTime &time, ZtdbValue &value ) {

	if ( mTimeValue.size ( ) == 0 ) {	// Nothing to get
		return false;					// Return false/no data
	}

	mTvIth = mTimeValue.begin ( );		// Else return 1st value
	time = mTvIth->first;
	value = mTvIth->second;

	return true;
}


bool ZtdbVar::getNext ( ZtdbTime &time, ZtdbValue &value ) {
	mTvIth++;

	if ( mTvIth == mTimeValue.end ( ) ) {	// If past end
		return false;					// Done - return false/no data
	}

	return true;
}


bool ZtdbVar::getAt ( ZtdbTime target, ZtdbTime &time, ZtdbValue &value ) {
	map<ZtdbTime, ZtdbValue>::iterator ith = mTimeValue.lower_bound ( target );

	if ( ith == mTimeValue.end ( ) ) {	// Past the last value
		return false;					// Done - return false/no data
	}

	time  = ith->first;					// Else return what was found
	value = ith->second;
	return true;
}

uint32_t ZtdbVar::getSize ( )
{
	return mSize;
}

uint32_t ZtdbVar::getNum ( )
{
	return uint32_t ( mTimeValue.size() );
}


float ZtdbVar::timePrecise ( ZtdbTime time, int precision ) {

	// Optimized for common precisions (ms, us, ns, ps)
	switch ( precision ) {
	case 0:  return ( float ( time ) );						break;
	case 3:  return ( float ( time ) / 1000LL );			break;
	case 6:  return ( float ( time ) / 1000000LL );			break;
	case 9:  return ( float ( time ) / 1000000000LL );		break;
	case 12: return ( float ( time ) / 1000000000000LL );	break;
	case 1:  return ( float ( time ) / 10 );				break;
	case 2:  return ( float ( time ) / 100 );				break;
	case 4:  return ( float ( time ) / 10000 );				break;
	case 5:  return ( float ( time ) / 100000 );			break;
	case 7:  return ( float ( time ) / 10000000 );			break;
	case 8:  return ( float ( time ) / 100000000 );			break;
	case 10: return ( float ( time ) / 10000000000 );		break;
	case 11: return ( float ( time ) / 100000000000 );		break;
	default: cout << "ZtdbVar::timePrecise: Die Die Die!!!\n";
	}
	return -1;
}


uint32_t ZtdbVar::getValueSize ( bool hex ) {
	if ( hex == 1 ) {
		return ( ( mSize + 3 ) / 4 );			// Round up
	} else { 
		if      ( mSize >= 30 ) { return 10; }	// Optimize for 32 bit values
		else if ( mSize <= 3  ) { return  1; }
		else if ( mSize <= 6  ) { return  2; }
		else if ( mSize <= 9  ) { return  3; }
		else if ( mSize <= 13 ) { return  4; }
		else if ( mSize <= 16 ) { return  5; }
		else if ( mSize <= 19 ) { return  6; }
		else if ( mSize <= 23 ) { return  7; }
		else if ( mSize <= 26 ) { return  8; }
		else                    { return  9; }
	}
}


string ZtdbVar::getValueString ( ZtdbValue value, bool hex ) {
	string result;

	uint32_t size = getValueSize ( hex );

	if ( value == ZTDB_UNKOWN ) {
		uint32_t repeat = size + ( hex * 2 );		// Hex usually prefixed with 0x
		result = string ( repeat, 'U' );
	} else if ( value == ZTDB_RANGE ) {
		uint32_t repeat = size + ( hex * 2 );		// Hex usually prefixed with 0x
		result = string ( repeat, 'R' );
	} else {
		char format[64];		// 64 chars should be enough...
		if ( hex == 1 ) {
			sprintf_s ( format, "0x%%0%dx", size );
			// DEBUG cout << "ZtdbVar::getValueString: format=" << format << "\n";
		} else {
			sprintf_s ( format, "%%%du", size );
		}
		char result_char[64];
		sprintf_s ( result_char, format, value );
		result = result_char;
	}
	return result;
}


void ZtdbVar::dump ( string prefix, uint32_t precision, bool hex ) {

	printf ( "%s # Time/Value pairs=%d\n", prefix.c_str(), getNum () );

	for ( auto const &x : mTimeValue ) {
		float pTime = timePrecise ( x.first, precision );
		string valueString = getValueString ( x.second, hex );

		char format[64];		// 64 chars should be enough...
		sprintf_s ( format, "%%s %%10.%df -> %%s\n", precision );

		printf ( format, prefix.c_str ( ), pTime, valueString.c_str ( ) );
	}
}

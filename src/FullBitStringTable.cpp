
#include <iostream>
#include <fstream>
#include <string>

#define NDEBUG
#include <cassert>
#include "FullBitStringTable.h"

using namespace std;





FullBitStringTable::FullBitStringTable( const int n,const string path) {
	FullBitStringTable(n, path.c_str() );
}


FullBitStringTable::FullBitStringTable(const int n ,const char * path) {
	base = n;
	table_size = (1 << base) ;
	table = new string[ table_size ];
	infile.open( path );
	if( !infile )
		cerr << "open error" << endl;
	else {
		if( !read(  ) )
			cerr << "error in file" << endl;
	}
}


bool FullBitStringTable::read() {
	int base_t;
	infile >> base_t;
	if( base_t != base) {
		cerr << "the base conflicts!" << endl;
		return false;
	}
	string *constriants = new string[base];
	for( int i = 0 ; i < base ; i++ ) {
		infile >> constriants[i];
	}
	for( int i = 0 ; i < table_size ; i++ ) {
		string s;
		for( int j = 0; j < base ; j++ ) {
			if( i & (1 << j) ) 
				s.append( constriants[j] + " ");
		}
		table[i] = s;
	}
	delete[] constriants;
	return true;
	
}


int FullBitStringTable::find( const string& s ) const {
	for( int i = 0; i < table_size ; i++ ) 
		if( s == table[i] ) 
			return i;
	return -1;
}


int FullBitStringTable::getBit(const string s) const {
	int pos = find( s );
	assert( pos >= 0 );
	int b;
	if( pos < 0) {
		cerr << s << " : " << "not legal constriant!" << endl;
	} else {
		b = pos;
	}
	return b;
}


string FullBitStringTable::getString( const int b)  const {
	return table[b];
}



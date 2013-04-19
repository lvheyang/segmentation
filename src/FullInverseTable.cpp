/**
 * record every constriant's inverse
 *
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "FullInverseTable.h"

using namespace std;

FullInverseTable::FullInverseTable( BitStringTable *b, string& path) {
	FullInverseTable( b, path.c_str());
}

FullInverseTable::FullInverseTable( BitStringTable *b, const char * path ) {
	bst = b;
	base = bst->getBase();
	table_size = 1 << base;
	infile.open( path );
	if( !infile ) 
		cerr << "inverse table construct error : open error" << endl;
	else {
		if(!read() ) 
			cerr << "invserse table construct error : read error" << endl;
	}

}

bool
FullInverseTable::read() {
	map< int , int > m_base;
	for( int i =0 ;i < base ; i++) {
		string c1, ci;
		int b_c1, b_ci;
		infile >> c1 >> ci;
		c1 += " ";
		ci += " ";
		b_c1 = bst->getBit( c1 );
		b_ci = bst->getBit( ci );
		m_base[ b_c1 ] = b_ci;
	}
	for( int i = 1; i < table_size; i++ ) {
		int value = 0;
	       	for(int j = 0; j < base ; j++ ) {
		       if( i & (1 << j)){
			       value |= (m_base[1<<j]);
		       }
	       	}
		m[ i ] = value;
	}	  
   	return true;	
}


int 
FullInverseTable::getInverse( int c ) {
	int p = m[c];
	return p;
}

/**
 *record all the results  of  
 * two constriants' composition
 *
 * this implementation is  time-saving 
 * and space-wasting 
 */

#include <iostream>
#include <fstream>
#include <string>

#include "FullCompositionTable.h"

using namespace std;

int
FullCompositionTable::composite(const int c1,const int c2 ) const{
	if( c1 >= (1<<base) || c2 >= (1<<base) \
			|| c1 < 0 || c2 < 0) { 
		cerr << "illegal relation" << endl;
		return -1;
	}
	return table[(c1 -1 )* column_size + c2 -1 ];
}


FullCompositionTable::FullCompositionTable(BitStringTable * b, string & path) {
	FullCompositionTable(b, path.c_str());
}

FullCompositionTable::FullCompositionTable(BitStringTable * b, const char * path) {
	base = b->getBase();
	column_size = (1 << base) - 1;
	table_size = column_size * column_size ;
	bst = b;
	table = new int[ table_size ];
	if( !table ) 
		cerr << "full composition table construct: error! out of memory" << endl;

	infile.open( path) ;
	if( !infile )
		cerr << "full composition table construct: error! open error" << endl;
	else {
		if( !read() ) 
			cerr << "full composition table construct: error! read error" << endl;
	}
}

bool
FullCompositionTable::read() {
	int base_t;
	infile >> base_t;
	if( base_t != base ) { 
		cerr << "base number conflict!" << endl;
		return false;
	}

	string line;

	getline( infile, line);
	getline( infile, line);

	while( getline(infile,line) ) {
		if( line == "EOF") {
			break;
		}
		string c1,c2,cr;
		int s1 = 0, e1 = 0, s2, e2, sr, er;
		while( line[e1++] != ':') ;
		c1 = line.substr( s1, e1 - s1 - 1 );
		e2 = s2 = e1 + 1;
		while( line[e2++] != ':') ;
		c2 = line.substr( s2, e2 - s2 - 1 );
		er = sr = e2 + 3;
		while( line[er++] != ')' ) ;
		cr = line.substr( sr, er - sr - 1 );
		cr += " ";
		int b_c1,b_c2,b_cr;
		b_c1 = bst->getBit( c1 );
		b_c2 = bst->getBit( c2 );
		b_cr = bst->getBit( cr );
		table[ (b_c1-1) * column_size + (b_c2 - 1) ] = b_cr;
	}
	
	for(int i = 0; i < column_size ; i++) {
		for( int j = 0 ; j < column_size ; j++ ) {
			if( !table[(i)*column_size + (j)]) {
				int comp_c = 0;
				for( int m = 0; m < base ; m++ ) {
					for( int n = 0; n < base ; n++ ) {
						if( ((i+1) & ( 1 << m)) && ( (j+1) & (1 << n))) {
							comp_c |= table[((1<<m) -1)*column_size + (1<<n)-1 ];
						}
						if( comp_c == ((1 << base) - 1)) 
							break;
					}
					if( comp_c == ((1 << base ) - 1 )) 
						break;
				}
				table[ (i)*column_size + j ] = comp_c;

			}
		}
	}
	return true;
}

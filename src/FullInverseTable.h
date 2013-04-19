#pragma once

#include <fstream>
#include <map>
#include "BitStringTable.h"
#include "InverseTable.h"

using namespace std;

class FullInverseTable :public InverseTable{
	public :
		FullInverseTable(BitStringTable *, string&   );
		FullInverseTable(BitStringTable *, const char * );
		virtual int getInverse( int ) ;
		virtual ~FullInverseTable() { infile.close();}
	private :
		bool read();
		map< int, int > m;
		int table_size;
		int base;
		BitStringTable *bst;
		ifstream infile;
};

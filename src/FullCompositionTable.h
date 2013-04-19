#pragma once

#include <fstream>

#include "BitStringTable.h"
#include "CompositionTable.h"

using namespace std;

class FullCompositionTable : public CompositionTable{
	public :
		FullCompositionTable( BitStringTable *, string & );
		FullCompositionTable( BitStringTable *, const char * );
		virtual int composite( const int, const int ) const;
		virtual ~FullCompositionTable() { infile.close(); delete[] table;}
	private :
		bool read() ;
		int base;
		int table_size;
		int column_size;
		BitStringTable * bst;
		int *table;
		ifstream infile;
};

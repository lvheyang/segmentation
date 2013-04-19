#pragma once
/**
 * record every constriant 
 * to its string representation
 *
 */
#include <fstream>
#include "BitStringTable.h"

using namespace std;


class FullBitStringTable : public BitStringTable {
	public :
		FullBitStringTable(const int , const string  );
		FullBitStringTable(const int , const char *  );
		virtual int getBit( const string) const; 
		virtual string getString( const int ) const;
		virtual ~FullBitStringTable() { infile.close() ; delete [] table;}
		virtual int getBase() const {return base;}
	private :
		bool read() ;
		int find( const string& ) const;
		int base;
		int table_size;
		string* table;
		ifstream infile;
};

#pragma once
/**
 * languagemodel 
 * use to collect the information ( composition table, inverse table, bit<-->string table)
 * of an RCC language
 */

#include "Constraint.h"
#include "CompositionTable.h"
#include "InverseTable.h"
#include "BitStringTable.h"


using namespace std;


class LanguageModel {
	public :
		LanguageModel(CompositionTable *c, InverseTable *i, BitStringTable *b ) {
			ct  = c;
			it = i;
			bst = b;
		}
		Constraint composite(const Constraint&, const Constraint& ) const ;
		Constraint getInverse( const Constraint& ) const ;
		Constraint getConstraint( const int  ) const;
	    Constraint getConstraint( const string  ) const;	
	private :
		CompositionTable *ct;
		InverseTable *it;
		BitStringTable *bst;
};

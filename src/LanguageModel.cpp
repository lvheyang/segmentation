#include <iostream>
#include "LanguageModel.h"

using namespace std;

Constraint
LanguageModel::composite( const Constraint& c1, const Constraint& c2 ) const {
	int b_c3 = ct->composite( c1.getBitset(), c2.getBitset() );	
	string s_c3 = bst->getString( b_c3 );
	Constraint c3( b_c3, s_c3);
	return c3;
}

Constraint
LanguageModel::getInverse(const Constraint& c ) const {
	int b_ci = it->getInverse(c.getBitset());
	string s_ci = bst->getString( b_ci );
	Constraint ci(b_ci, s_ci);
	return ci;
}
	
Constraint
LanguageModel::getConstraint( const int b_c ) const {
	string s_c = bst->getString( b_c );
	Constraint ci( b_c, s_c);
	return ci;
}

Constraint
LanguageModel::getConstraint( const string s_c ) const {
	int b_c = bst->getBit(s_c);
	Constraint ci(b_c, s_c);
	return ci;
}

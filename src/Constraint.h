#pragma once

#include <string>

using namespace std;


class Constraint {
	public :
		Constraint() {  bit_cstr = 0; s_cstr = "" ;}
		Constraint( const int b, string s ) { bit_cstr = b; s_cstr = s;}
		int getBitset() const { return bit_cstr ;}
		string getStr() const { return s_cstr; }

		int revalue( Constraint c ) {
			bit_cstr = c.getBitset();
			s_cstr = c.getStr();
		}
	private : 
		string s_cstr;
		int bit_cstr;
};

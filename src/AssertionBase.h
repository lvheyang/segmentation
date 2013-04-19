#pragma once

#include <string>
#include <list>
#include "Explain.h"

using namespace std;

class AssertionBase {
	public :
		AssertionBase( const string path ) {
			AssertionBase( path.c_str());
		}
		AssertionBase( const char *);
		bool hasNxtExpl() {return i_l!=l.end();}
		Explain& getNxtExpl() { return *(i_l++);}
		void begin() { i_l = l.begin();}
	private :
		list<Explain> l;
	    list<Explain>::iterator i_l;
};

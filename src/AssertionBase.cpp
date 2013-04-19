#include <list>
#include <fstream>
#include "Explain.h"
#include "AssertionBase.h"

using namespace std;

AssertionBase::AssertionBase( const char * path) {
	ifstream infile(path);

	string sub;
	string obj;
	string pro;
	
	while(!infile.eof()) {
		infile >> sub >> obj >> pro;
		Explain expl( sub, obj,pro);
		l.insert( l.end(), expl);
	}

	i_l = l.begin();
}

#pragma once

#include <string>
#include "Explain.h"
#include "AssertionBase.h"

using namespace std;

class ErrorExplain {
	public :
		ErrorExplain( AssertionBase * ab) { base = ab; }
		list<Explain> getAllExpls(const string, const string ) ;

	private :
		AssertionBase * base;
};


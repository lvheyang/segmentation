#pragma once

#include <vector>
#include <fstream>
#include <list>
#include "ALConstraintGraph.h"

using namespace std;


class MultiCSPFile {
	public :
		MultiCSPFile( list<ALConstraintGraph> * p ) { lptr = p ; }

		void exportToFile( const char * csppath ) const;
	private :
		list<ALConstraintGraph> * lptr;
};

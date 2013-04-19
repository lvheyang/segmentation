#pragma once

#include <string>
#include <vector>
#include "LanguageModel.h"
#include "Arc.h"
#include "Constraint.h"


using namespace std;


class Vertex {
	public :
		Vertex( const int id, const string n ) {
			vid = id;
			name = n;
		}

		int getVid(void) const { return vid ; }
		string getName( void ) const { return name; }
		vector<Arc> getArcs( void) const { return arcs; }
		bool addArc( int obj, Constraint c, LanguageModel *l ) ;
		bool delArc( int obj ) ;

	private :

		int vid;
		string name;

		vector<Arc> arcs ;
};

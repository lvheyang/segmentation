#pragma once

#include <string>
#include "Constraint.h"
#include "Vertex.h"

using namespace std;

class Arc {
	public :
		Arc ( const int id, const Constraint c ) {
			adjvid = id;
			constraint = c ;
		}

		int getAdj(void) const { return adjvid ; }
		Constraint getConstraint(void) const {return constraint;}
		void setConstraint( Constraint c ) {
			constraint = c;
		}
	private :
		/**
		 * 一个点的邻接边，包括邻接的点的id和邻接的约束
		 */
		int adjvid ;
		Constraint constraint ;
};

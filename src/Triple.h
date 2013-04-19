#pragma once

#include "Constraint.h"

class Triple {
	public :
		Triple(int i, int j, Constraint c ) {
			id1 = i;
			id2 = j;
			constraint = c;
		}
		int getId1(void) const {return id1 ; }
		int getId2(void) const {return id2 ; }
		Constraint getConstraint(void) const {return constraint;}

	private :
		int id1;
		int id2;
		Constraint constraint;
};


#include "Vertex.h"

bool
Vertex::addArc(int obj, Constraint c, LanguageModel *l ) {
	/**
	 * return true 如果添加成功
	 */
	int size = arcs.size();
	bool has_contained = false;
	for(int i = 0 ; i < size ; i++ ) {
		if( arcs[i].getAdj() == obj ) {
			Constraint old_c = arcs[i].getConstraint() ;
			int new_c_bit = old_c.getBitset() & c.getBitset() ;
			if( new_c_bit != old_c.getBitset() ) {
				Constraint new_c = l->getConstraint( new_c_bit );
				arcs[i].setConstraint( new_c );
			}
			has_contained = true;
			break;
		}
	}
	if( !has_contained ) {
		Arc arc( obj, c);
		arcs.push_back( arc );
	}
	return !has_contained;
}


bool
Vertex::delArc( int obj ) {
	/**
	 * return true 如果删除成功
	 */
	int size = arcs.size();
	bool del_success = false;
	for( int i = 0; i < size ; i++ ) {
		if( arcs[i].getAdj() == obj ) {
			vector<Arc>::iterator pos;
			pos = arcs.begin() + i;
			arcs.erase( pos );
			del_success = true;
			break;
		}
	}
	return del_success;
}

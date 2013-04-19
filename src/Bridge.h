#pragma once

#include <utility>
#include "ALConstraintGraph.h"



class Bridge {
	public :
		Bridge(ALConstraintGraph *g);
		
		int getBridgeNum( void ) const {return bridges ;}

		vector< pair<int,int> > getBridges( void) const { return bridges_record; }
		
		void delBridges( void ) ;
		bool bridgeIsDeleted() {return deleted;}

	private :
		int bridges ; // number of bridges 
		int cnt; // counter
		vector<int> pre; // pre[v] = order in which dfs examines v
		vector<int> low; // low[v] = lowest preorder of any vertex connected to v
		vector< pair<int, int> > bridges_record; 

		bool deleted;
		ALConstraintGraph *alg;

		void dfs( ALConstraintGraph *alg,int u, int v);
		void dfs_iter( ALConstraintGraph *alg,int u, int v);
};

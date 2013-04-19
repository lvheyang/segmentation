#pragma once

#include <vector>
#include "ConstraintNetwork.h"
#include "ALConstraintGraph.h"
#include "util.h"


class ConnectComponent {
	public :
		ConnectComponent( ALConstraintGraph *alg, LanguageModel *l );

		int getId( int v ) const { return id[v] ; }
		int getSize( int gid ) const { return size[gid];}
		int getSizeOfVertex( int v ) const { return size[id[v]];}
		int getCount(void ) const { return count; }
		bool areConnected( int v, int w ) { return id[v]==id[w]; }
		
		vector<ALConstraintGraph> getAllComponents( void) const; 
		void exportToFile( void) const; 
	private :
		LanguageModel *lm; 
		ALConstraintGraph *G;
		vector<bool> marked; // marked[v] = has vertex v been marked?
		vector<int> id; // id[v] = id of connected component containing v
		vector<int> size; // size[id] = number of vertices in given component
		int count; // number of connected comppnents

		virtual void dfs(ALConstraintGraph *G , int v ) ;
		virtual void dfs_iter(ALConstraintGraph *G , int v ) ;
};

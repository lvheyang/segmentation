
#include <stack>
#include "ConnectComponent.h"

//#define DBG
#ifdef DBG
#include <iostream>
#endif

/**
 * public
 */
ConnectComponent::ConnectComponent( ALConstraintGraph *alg ,LanguageModel *l ) {
	G = alg;
	lm = l;
	count = 0;
	for( int i = 0; i < G->getVexnum(); i++ ) {
		marked.push_back( false );
		id.push_back( 0 );
		size.push_back( 0 );
	}
	for( int v = 0; v < G->getVexnum(); v++ ) {
		if( !marked[v]) {
			dfs_iter(alg, v);
			count++;
		}
	}

}

vector<ALConstraintGraph> 
ConnectComponent::getAllComponents( void) const {
	vector<ConstraintNetwork> networks;
    vector<ALConstraintGraph> graphs;
	for( int i =0; i < count; i++ ) {
		ConstraintNetwork c( lm );
		networks.push_back( c );
	}
	for( int i =0; i < id.size(); i++ ) {
		networks[id[i]].addVertex(G->getVexname(i));
	}
    for( int i =0; i < id.size(); i++ ) { 
		string name = G->getVexname( i ) ;
		vector<Arc> adj = G->getAdjacentArcs(i);
		for( int j = 0; j < adj.size();j++ ) {
			string adj_name = G->getVexname(adj[j].getAdj());
			Constraint c = adj[j].getConstraint();
			networks[id[i]].addConstraint( name, adj_name, c );
		}
	}
	for( int i = 0; i < networks.size(); i++ ) {
		ALConstraintGraph alg( &(networks[i]), lm);
        graphs.push_back( alg );
	}
    return graphs;
    
}


void 
ConnectComponent::exportToFile( void ) const {
	string prefix = "component";
	string csp = ".csp";
	string map = ".map";
	vector<ConstraintNetwork> networks;
	for( int i =0; i < count; i++ ) {
		ConstraintNetwork c( lm );
		networks.push_back( c );
	}
	for( int i =0; i < id.size(); i++ ) {
		networks[id[i]].addVertex(G->getVexname(i));
	}
	for( int i =0; i < id.size(); i++ ) { 
		string name = G->getVexname( i ) ;
		vector<Arc> adj = G->getAdjacentArcs(i);
		for( int j = 0; j < adj.size();j++ ) {
			string adj_name = G->getVexname(adj[j].getAdj());
			Constraint c = adj[j].getConstraint();
			networks[id[i]].addConstraint( name, adj_name, c );
		}
	}
	for( int i = 0; i < networks.size(); i++ ) {
		string no = itoa(i,10);
		networks[i].exportToFile( prefix+no+csp , prefix+no+map );
	}
}

/**
 * private
 *
 */
void
ConnectComponent::dfs_iter( ALConstraintGraph *G, int va ) {
	stack<int> dstack;
	dstack.push(va);
	while( !dstack.empty()) {
		int v = dstack.top();
		dstack.pop();
		marked[v] = true;
		id[v] = count;
		size[count]++;
		vector<int> adj = G->getAdjacent(v);
		for( int i = 0; i < adj.size(); i++ ) {
			int w = adj[i];
			if(!marked[w]) {
				dstack.push(w);
			}
		}
	}
}


void
ConnectComponent::dfs( ALConstraintGraph *G, int v ) {
	
	marked[v] = true;
	id[v] = count;
	size[count]++;
	vector<int> adj = G->getAdjacent( v );
	for( int i = 0; i < adj.size(); i++ ) {
		int w = adj[i];
		if(!marked[w]) {
			dfs(G, w );
		}
	}
}

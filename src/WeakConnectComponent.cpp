
#include <stack>
#include "WeakConnectComponent.h"

//#define DBG
#ifdef DBG
#include <iostream>
#endif

/**
 * public
 */
WeakConnectComponent::WeakConnectComponent( ALConstraintGraph *alg ,LanguageModel *l, Constraint c ) {
	weak_relation = c;
	G = alg;
	lm = l;
	count = 0;
	for( int i = 0; i < G->getVexnum(); i++ ) {
		marked.push_back( false );
		id.push_back( 0 );
		size.push_back( 0 );
	}
#ifdef DBG
	cout << "vex num " << G->getVexnum() << endl;
	cout << "arc num " << G->getArcnum() << endl;
#endif 
	for( int v = 0; v < G->getVexnum(); v++ ) {
		if( !marked[v]) {
			dfs_iter(alg, v, c);
			count++;
		}
	}

}

vector<ALConstraintGraph> 
WeakConnectComponent::getAllComponents( void ) const {
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
			int adj_id = adj[j].getAdj() ;
			if( id[i] == id[adj_id]) {
				string adj_name = G->getVexname(adj_id);
				Constraint c = adj[j].getConstraint();
				networks[id[i]].addConstraint( name, adj_name, c );
			}
		}
	}
	for( int i = 0; i < networks.size(); i++ ) {
		ALConstraintGraph alg(&(networks[i]), lm);
        graphs.push_back( alg );
	}
    
    return graphs;
}
void 
WeakConnectComponent::exportToFile( void ) const {
	string prefix = "weakcomponent";
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
			int adj_id = adj[j].getAdj() ;
			if( id[i] == id[adj_id]) {
				string adj_name = G->getVexname(adj_id);
				Constraint c = adj[j].getConstraint();
				networks[id[i]].addConstraint( name, adj_name, c );
			}
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
WeakConnectComponent::dfs_iter( ALConstraintGraph *G, int va, Constraint c ) {
	stack<int> dstack;
	dstack.push(va);
	while( !dstack.empty()) {
		int v = dstack.top();
		dstack.pop();
		marked[v] = true;
		id[v] = count;
		size[count]++;
		vector<int> adj = G->getAdjacentBy(v,c );
#ifdef DBG 
		cout << adj.size() << endl;
#endif
		for( int i = 0; i < adj.size(); i++ ) {
			int w = adj[i];
			if(!marked[w]) {
				dstack.push(w);
			}
		}
	}
}


void
WeakConnectComponent::dfs( ALConstraintGraph *G, int v, Constraint c ) {
	
	marked[v] = true;
	id[v] = count;
	size[count]++;
	vector<int> adj = G->getAdjacentBy( v, c );
	for( int i = 0; i < adj.size(); i++ ) {
		int w = adj[i];
		if(!marked[w]) {
			dfs(G, w, c );
		}
	}
}

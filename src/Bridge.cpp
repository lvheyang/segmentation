
#include <stack>
#include "Bridge.h"

//#define DBG

#ifdef DBG
#include "iostream"
#endif

/**
 * public 
 */
Bridge::Bridge(ALConstraintGraph *g ) {
	alg = g;
	deleted = false;
	bridges = 0;
	cnt = 0;

#ifdef DBG
	cout << "vex num : " << g->getVexnum()  << endl;
	cout << "arc num : " << g->getArcnum()  << endl;
#endif
	int vexnum = alg->getVexnum();

	for( int v = 0; v < vexnum; v++ ) {
		low.push_back( -1 );
	}
	for( int v = 0; v < vexnum; v++ ) {
		pre.push_back( -1 );
	}

	for( int v = 0 ; v < vexnum; v++ )
		if( pre[v] == -1) { 
			dfs_iter(alg, v, v);
#ifdef DBG
			cout << "Bridge.cpp component: " << v << endl;
#endif
		}
}

void
Bridge::delBridges( void ) {
	for( int i = 0; i < bridges_record.size(); i++) {
		int u = bridges_record[i].first;
		int v = bridges_record[i].second;

		alg->delEdge( u, v );

	}
	deleted = true;
}

/**
 * private
 */
#if 1
void 
Bridge::dfs_iter(ALConstraintGraph *g , int ua , int va) {
		
	stack<pair<int,int> > dstack;
	stack<pair<int,int> > compare_stack;
	dstack.push(pair<int,int>(ua,va));
	while( !dstack.empty() ) {
		pair<int,int> arc = dstack.top();
		dstack.pop();
		int u = arc.first;
		int v = arc.second;
		
		if( pre[v] != -1) {
			continue;
		}

		pre[v] = cnt++;
		low[v] = pre[v];
#ifdef DBG
        cout << "pop: " << u << " " << v << endl;
#endif
		vector<int> adj = g->getAdjacent(v) ;
		for( int i = 0; i < adj.size() ; i++ ) {
			int w = adj[i];
			if( pre[w] == -1 ) {
				dstack.push(pair<int,int>(v,w));
                compare_stack.push(pair<int,int>(v,w));
#ifdef DBG
                cout << "push: " << v << " " << w << endl;
#endif
			}
			else if ( w!=u ) {
#ifdef DBG
                int previous = low[v];
#endif
				low[v] = low[v]<pre[w] ? low[v]:pre[w];
#ifdef DBG
                if ( low[v] != previous ) {
                    cout << w << "!=" << u << ": low[" << v << "] changed to "  << low[v] << endl ;
                } else {
                    cout << w << "!=" << u << ": but low[" << v << "] did not change"<<  endl ;
                }
#endif
			}
		}
	}

    while( !compare_stack.empty() ) {
        pair<int,int> compare = compare_stack.top();
        compare_stack.pop();
        int v = compare.first;
        int w = compare.second;
        low[v] = low[v] < low[w] ? low[v] :low[w];
        if( low[w] == pre[w] ) {
            bridges_record.push_back( pair<int,int>(v,w));
            bridges++;
#ifdef DBG
			cout << v << "-" << w << " bridges++" << endl;
#endif
        }
    }

#ifdef DBG
	for( int i = 0 ; i < g->getVexnum(); i++ ) {
		cout << i << " pre: "  << pre[i] ;
		cout << " low: "  << low[i] << endl ;
	}
#endif
}
#endif

void
Bridge::dfs(ALConstraintGraph *g, int u ,int v) {
	pre[v] = cnt++;
	low[v] = pre[v];
	vector<int> adjacent = g->getAdjacent(v);
	for(int i = 0; i < adjacent.size(); i++ ) {
		int w = adjacent[i];
		if( pre[w] == -1 ) {
			dfs(g, v, w);
			low[v] = (low[v] < low[w]) ? low[v]  : low[w] ;
			if( low[w] == pre[w] ) {
				bridges_record.push_back( pair<int,int>(v,w));
				bridges++;
			}
		}
		// update low number - ignore reverse of edge leading to v
		else if ( w!=u )
			low[v] = (low[v] < pre[w]) ? low[v] : pre[w] ;
	}
}

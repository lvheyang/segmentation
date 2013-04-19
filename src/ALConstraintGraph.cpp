
#include "ALConstraintGraph.h"

//#define DBG

#ifdef DBG
#include <iostream>
#endif

ALConstraintGraph::ALConstraintGraph( ConstraintNetwork *cn, LanguageModel *l ) {
	vexnum = cn->getNamenum();
	arcnum = 0;
	lm = l;
	/**
	 * 依次加入所有节点
	 */
	int vex_amount = cn->getNamenum() ;
	for( int i = 0; i < vex_amount ; i++ ) {
		Vertex v( i, cn->getNameAt(i) );
		vertices.push_back( v );
	}

	/**
	 * 依次加入所有边
	 */
	int triple_amount = cn->getTripleNum() ;
	for ( int i = 0; i < triple_amount ; i++ ) {
#ifdef DBG
	cout << arcnum << endl;
#endif
		Triple t = cn->getTripleAt(i);
		addEdge( t );
	}
}


vector<int> 
ALConstraintGraph::getAdjacentBy( int m, int rel ) const {

	vector<int> adjacent;
	vector<Arc> arcs = vertices.at(m).getArcs() ;

	for ( int i =0; i < arcs.size() ; i++ ) {
#ifdef DBG
		cout << "i_th  " << i << endl;
		cout << "arcs.size() " << arcs.size() << endl;
		cout << "arcs[i].getConstraint().getBitset() " <<arcs[i].getConstraint().getBitset() << endl;
		cout << "arcs[i].getAdj() " <<arcs[i].getAdj() << endl;
#endif
		if( ! (arcs[i].getConstraint().getBitset() & rel ) )  {
			adjacent.push_back(arcs[i].getAdj());
		}
	}
#ifdef DBG
	cout << "adjacent size " << adjacent.size() << endl;
#endif
	return adjacent;
}


vector<int> 
ALConstraintGraph::getAdjacent( int m ) const {
	vector<int> adjacent;
	vector<Arc> arcs = vertices.at(m).getArcs() ;

	for ( int i =0; i < arcs.size() ; i++ ) {
		adjacent.push_back(arcs[i].getAdj());
	}
	return adjacent;
}

void 
ALConstraintGraph::addEdge( Triple t ) {
	int sub = t.getId1();
	int obj = t.getId2();
	Constraint c = t.getConstraint();
	
	bool add_success = vertices.at(sub).addArc( obj, c, lm );
	vertices.at(obj).addArc( sub, lm->getInverse(c) , lm );
	
	if(add_success) 
		arcnum++;

}
void 
ALConstraintGraph::delEdge( int m , int  n ) {
	bool del_success = vertices.at(m).delArc(n);
	vertices.at(n).delArc(m);

	if(del_success) 
		arcnum--;
}

vector<string> 
ALConstraintGraph::toCSPString( int no ) const {
	vector<string> strings;
	ostringstream stream; 
	stream << vexnum << " # " << no << endl;
	for( int sub = 0 ; sub < vertices.size(); sub++) {
		vector<Arc> arcs = vertices.at(sub).getArcs() ;
		for( int j = 0; j < arcs.size() ; j++ ) {
			int obj = arcs.at(j).getAdj();
			string c = arcs.at(j).getConstraint().getStr();
			stream << sub << " " << obj << " ( " << c << ")" << endl;
		}
	}
	strings.push_back( stream.str() );
	strings.push_back( string(".\n"));
	return strings;
}


void 
ALConstraintGraph::exportToFile( const char * csp_path, const char * map_path ) const{
	ofstream ocsp_file, omap_file;
	ocsp_file.open( csp_path );
	omap_file.open( map_path );
	
	ocsp_file << vexnum << "   #" << csp_path << endl;

	for( int sub = 0 ; sub < vertices.size(); sub++) {
		vector<Arc> arcs = vertices.at(sub).getArcs() ;
		for( int j = 0; j < arcs.size() ; j++ ) {
			int obj = arcs.at(j).getAdj();
			string c = arcs.at(j).getConstraint().getStr();
			ocsp_file << sub << " " << obj << " ( " << c << ")" << endl;
		}
	}
	ocsp_file << ".";

	for( int sub = 0; sub < vertices.size(); sub++ ) {
		omap_file << sub << " " << vertices[sub].getName() << endl;
	}
	omap_file << -1;

    ocsp_file.close();
    omap_file.close();
}       



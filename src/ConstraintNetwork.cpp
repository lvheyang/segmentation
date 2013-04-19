
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>
#include "ConstraintNetwork.h"

//#define DBG

#ifdef DBG
#include <iostream>
#endif

/**
 * public
 */
ConstraintNetwork::ConstraintNetwork( int V , int E, LanguageModel *l){
	srand(time(NULL));
	vexnum = 0;
	lm = l;
	for( int i = 0; i < V ; i++ ) {
		addVertex( itoa( i+1 , 10) );
	}
	for( int i = 0; i < E ; i++ ) {
		int v = (int) ( rand() % V) ;
		int w = (int) ( rand() % V) ;
		int c = (int) ( rand() % 255) + 1;
		Constraint constraint = l->getConstraint( c );
		addConstraint( v, w, constraint);
	}
}

void 
ConstraintNetwork::addVertex( string name ) {
	if( names.size() <= 0 ) {
		vector<string>::iterator it;
		it = names.begin();
		names.insert(it,name);
		vexnum++;
		return;
	}

	int pos = binsearch( name );
	vector<string>::iterator it ;
	if(  names.at(pos) > name  ) {
		it = names.begin() + pos;
		names.insert(it, name);
		vexnum++;
	}
	else if(  names.at(pos) < name  ) {
		it = names.begin() + pos + 1;
		names.insert(it, name);
		vexnum++;
	}
}

void 
ConstraintNetwork::addVertices( vector<string>  name_set ) {
	int size = name_set.size();
	for ( int i = 0; i < size ; i++ ) {
		addVertex(name_set[i]);
	}
}

void 
ConstraintNetwork::addConstraint( string name1, string name2, Constraint c ) {
	int id1 = getVindex(name1);
	int id2 = getVindex(name2);
	Triple t( id1, id2, c );
	triples.push_back( t );
}

void 
ConstraintNetwork::addConstraint( int v, int w, Constraint c ) {
	Triple t( v, w, c );
	triples.push_back( t );
}

int 
ConstraintNetwork::getVindex( string name ) const {
	int pos = binsearch(name);

	if( names.at(pos) != name ) 
		return -1;
	else 
		return pos;
}

bool
ConstraintNetwork::exportToFile( const char * csppath, const char * mappath ) const{
	ofstream map_file, csp_file;
	map_file.open( mappath);
	csp_file.open( csppath);
	
	if( !map_file || !csp_file ) 
		return false;

	for( int i = 0; i < names.size(); i++ ) {
		map_file << i << " " << names[i] << endl;
	}

	map_file << -1 ;
	
	int num = getNamenum();
	csp_file << num << " # " << csppath << endl;
	for( int i = 0 ; i < getTripleNum() ; i++) {
		Triple t = triples[i];
		csp_file << t.getId1() << " " << t.getId2() << " ( " \
			<< t.getConstraint().getStr() << ")"  << endl;
	}
	csp_file << "." ;
	return true;
}

  

/**
 * private
 */
bool
ConstraintNetwork::read(const char * csppath, const  char * mappath ) {
	ifstream map_file, csp_file;
	map_file.open( mappath);
	csp_file.open( csppath);
	map<int,string> id_map;
	

	if( !(map_file && csp_file))
		return false;
	
	while( true ) {
		/**
		 *read the vertices' id and name
		 */
		int id;
		string name;
		map_file >> id >> name;
		if( -1 == id )
			break;
		addVertex( name);	
		id_map.insert( pair<int,string> (id ,name) );
	}


	string line;
	getline( csp_file, line );
	while( true ) {
		/**
		 *read the constriants' between
		 *vertices
		 */
		int n1, n2;
		string constriant;
		string temp;

		getline( csp_file , line );
		if(line == ".")
			break;

		istringstream iss( line);

		iss >> n1 >> n2;

		while( iss >> temp) {
			constriant.append( temp + " ");
		}
		
		constriant = constriant.substr( 2, constriant.length() - 4) ;
		
		Constraint c = lm->getConstraint(constriant);

		Triple t( getVindex(id_map.find(n1)->second) , 
				getVindex(id_map.find(n2)->second) , 
				c );
		triples.push_back( t );
	}
#ifdef DBG
	cout << getNamenum() << endl;
#endif
	return true;
}

int 
ConstraintNetwork::binsearch( string name ) const {
	if( names.size() <= 0 ) {
		return 0;
	}


	int start =0;
	int end = names.size() - 1;
	int mid = (start+ end) /2 ;

	while( names.at(mid) != name && start < end ) {
		if( names.at(mid).compare( name ) > 0) {
			end = mid -1;
		}
		else if ( names.at(mid).compare( name ) < 0 ) {
			start = mid +1;
		}

		mid = (start + end) /2;

	}

	return mid;
}

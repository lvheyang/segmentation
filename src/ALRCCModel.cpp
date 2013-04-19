/**
 *implementation of adjacent linked graph
 * 
 *
 */
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <vector>
#include <string>
#include <assert.h>
#include "ALRCCModel.h"

using namespace std;

void
ALRCCModel::setConstraint( int sub, int obj, int relation, list<ConstraintArc> * lca ) const {
	if(lca->empty()) {
		Constraint cnstr = lm->getConstraint(relation) ;
		ConstraintArc ca( sub, obj , cnstr ) ;
		lca->insert(lca->end(), ca);
		return;
	}
	list<ConstraintArc>::iterator it = lca->begin();
	bool founded = false;
	while( it != lca->end() ) {
		int lsub = it->getSub() ;
		int lobj = it->getObj() ;
		Constraint c = it->getConstraint() ;
		int relation_old = c.getBitset();
		if( (lsub==sub)&& (lobj==obj) ) {
			int relation_new = relation & relation_old;
			if( relation_new != relation_old ) {
				Constraint c_new = lm->getConstraint( relation_new );
				it->setConstraint( c_new );
			}
			founded=true;
			break;
		}
		it++;
	}
	if( !founded ) {
		Constraint cnstr = lm->getConstraint(relation) ;
		ConstraintArc ca( sub, obj, cnstr );
		lca->insert( lca->end() ,ca );
	}
}



RegionNode
ALRCCModel::getRegionNodeById( int n ) const {
	for ( int i = 0; i < vertices.size(); i++ ) {
		if( vertices[i].getVid()==n )
			return vertices[i];
	}
	
	return  RegionNode();
}

void
ALRCCModel::check_inverse_error() const {
	int amount;
	for( int i = 0; i < vertices.size(); i++ ) {
		list<ArcNode> arcs = vertices[i].getArcNodes();
		list<ArcNode>::iterator arcs_it = arcs.begin();
		while( arcs_it != arcs.end() ) {
			int relation_ij = (arcs_it->getConstraint()).getBitset();
			int obj = arcs_it->getAdjvex();
			string s_c = arcs_it->getConstraint().getStr();
			
			arcs_it++;
			vector<string> put_info1;
			vector<string> put_info2;
			while( arcs_it->getAdjvex() == obj ) {
				relation_ij &= arcs_it->getConstraint().getBitset();
				put_info1.push_back( arcs_it->getConstraint().getStr()) ;
				arcs_it++;
			}
			list<ArcNode> inverse_arcs = vertices[obj].getArcNodes();
			list<ArcNode>::iterator inverse_arcs_it = inverse_arcs.begin();
			while( inverse_arcs_it!= inverse_arcs.end() ) {
				int adj = inverse_arcs_it->getAdjvex();
				if( i == adj ) {
					relation_ij &= (lm->getInverse( inverse_arcs_it->getConstraint() )).getBitset();
					put_info2.push_back( inverse_arcs_it->getConstraint().getStr()) ;
				}
				inverse_arcs_it++;

			}
			if( relation_ij == 0 ) {
				amount++;
				cout << "inverseerror" << endl;
				cout << i << " " << obj << " ( " << s_c  << ")"<< endl;
				for ( int m = 0; m < put_info1.size() ; m++ ) {
					cout << i << " " << obj << " ( " << put_info1[m] << ")" << endl;
				}
				for( int m =0 ; m < put_info2.size() ; m++ ) {
					cout << obj << " " << i << " ( " << put_info2[m] << ")" << endl;
				}
				//cout << vertices[i].getName() << " " << vertices[obj].getName() << endl;
			}
			
			
			
		}
	}
}

void 
ALRCCModel::check_multi_csp() const {
	int amount = 0;
	for( int i = 0; i < vertices.size(); i++) {
		list<ArcNode> arcs = vertices[i].getArcNodes();
		list<ArcNode>::iterator arcs_it = arcs.begin();
		while( arcs_it != arcs.end() ) {
			int multi_r = (arcs_it->getConstraint()).getBitset();

			int obj = arcs_it->getAdjvex();
			int b_c = arcs_it->getConstraint().getBitset();
			string s_c = arcs_it->getConstraint().getStr();
			arcs_it++;
			vector<string> s_temp;
			/*
			if(arcs_it->getAdjvex() == obj) {
				multi_r &= (arcs_it->getConstraint()).getBitset();
			}*/
			while( (arcs_it->getAdjvex()) == obj ) {
				multi_r &= (arcs_it->getConstraint()).getBitset();
				s_temp.push_back( arcs_it->getConstraint().getStr()); 
				arcs_it++;
			}
			if(!multi_r) {
				amount++;
				cout << "multicsp" << endl;
				cout << i << " " << obj << " ( " << s_c  << ")"<< endl;
				for( int loop = 0 ; loop < s_temp.size() ; loop++ ) {
					cout << i << " " << obj << " ( " << s_temp[loop] << ")" << endl;
				}
				//cout << vertices[i].getName() << " " << vertices[obj].getName() << endl;
			}
		}
	}
	//cout << amount << endl;
}


void
ALRCCModel::print() const {
	for(int i = 0 ; i < vertices.size() ; i++) {
		cout << vertices[i].getVid() << " " << vertices[i].getIndeg() << " " << vertices[i].getOutdeg() << endl;
		if( 12665 == i ) {
			cout << "&&&" << endl;
			list<ArcNode> arcs = vertices[i].getArcNodes();
			list<ArcNode>::iterator it = arcs.begin();
			while( it != arcs.end() ) {
				cout << it->getAdjvex() << " ";
				it++;
			}
			cout << endl;
		}
	}
}

list<int>
ALRCCModel::findConnVex( int headNo , int relation ) const{
	list<int> conn_vexs ;
	for( int i = 0 ; i < vertices.size() ; i++ ) {
		list<ArcNode> arcs = vertices[i].getArcNodes();
		if( i == headNo ) {
			list<ArcNode>::iterator a_it = arcs.begin() ;
			while( a_it != arcs.end()) {
				int c = (a_it->getConstraint()).getBitset();
				if(!( (relation & c) == relation )) {
					conn_vexs.insert( conn_vexs.end() , a_it->getAdjvex());
				}
				a_it++;
			}
		} else {
			list<ArcNode>::iterator a_it = arcs.begin() ;
			while( (a_it != arcs.end()) ) {
				if( ((a_it->getAdjvex()) == headNo)) {
					int c = (a_it->getConstraint()).getBitset();
					if(!( (relation & c) == relation) ) {
						conn_vexs.insert( conn_vexs.end(), i );
						break;
					}
				}
				a_it++;
			}
		}
	}

	return conn_vexs;
}

list<ALRCCModel> 
ALRCCModel::single_segmentation( const int b_relation ) const {
	list<ALRCCModel> subgraphs;

	vector<bool> visited(vertices.size());

	for( int i = 0; i < visited.size(); i++ ) {
		visited[i] = false;
	}

	queue<int> queue_node ;

	for( int i = 0; i < vertices.size(); i++ ) {
		list<RegionNode> new_node_list ;
		map<int , int > mapping;
		int no = 0;

		if( !visited[i] ) {
			visited[i] = true;
			new_node_list.insert( new_node_list.end(), vertices[i] ) ;
			mapping[vertices[i].getVid()] = ++no;

			queue_node.push( i );
			
			while( !queue_node.empty() ) {
				int headNo = queue_node.front();
				queue_node.pop();

				list<int> conn_vexs = findConnVex( headNo, b_relation );
				list<int>::iterator cv_it = conn_vexs.begin();

				while(cv_it != conn_vexs.end()) {
					int vex = *cv_it;
					if( !visited[vex] ) {
						visited[vex] = true;
						new_node_list.insert( new_node_list.end(), vertices[vex]);
						mapping[vertices[vex].getVid()] = ++no;
						queue_node.push( vex);
					}
					cv_it++;
				}

			}


			list<ConstraintArc> n_lca;
			list<RegionNode>::iterator n_it = new_node_list.begin();
			int n_sub = 0;
			while( n_it != new_node_list.end() ) {
				list<ArcNode> arcs = n_it->getArcNodes();
				list<ArcNode>::iterator a_it = arcs.begin();
				while( a_it != arcs.end()) {
					int p_obj = a_it->getAdjvex();
					int n_obj = mapping[ vertices[p_obj].getVid()];
					if( !n_obj) {
						a_it++;
						continue;
					}
					Constraint c = a_it->getConstraint();
					ConstraintArc n_ca( n_sub, n_obj -1 , c);
					n_lca.insert( n_lca.end(), n_ca);
					a_it++;
				}

				n_sub++;
				n_it++;
			}

			n_it = new_node_list.begin();
			while( n_it != new_node_list.end()) {
				n_it->clearArcNodes();
				n_it->setDegZero();
				n_it++;
			}
			ALRCCModel a_rmodel( new_node_list, n_lca, lm);

			subgraphs.insert( subgraphs.end(), a_rmodel);


		}


	}	
	
	cout << "amount of cliques: " << subgraphs.size() << endl;
	return subgraphs;
}

ALRCCModel 
ALRCCModel::delete_node_erasable( int deg_limit ) const {
	/*
	 * delete the node erasable m
	 * ust be used after preprocess()
	 */
	list<RegionNode> new_node_list;
	map<int, int> mapping;
	int no = 0;
	list<ConstraintArc> n_lca;
	
	for(int i = 0; i < vertices.size() ; i++ ) {
		bool isolate = true;
		int indeg = vertices[i].getIndeg();
		int outdeg = vertices[i].getOutdeg();
		assert( indeg==outdeg) ;
		int deg = 0;
		if(indeg == outdeg ) {
			deg = indeg;
		}else {
			cout << "Warning: did you use preprocess function? the result may be not correct" << endl;
		}

		if(deg==1) {
			isolate = true;
		} else if ( deg < deg_limit) {
			list<ArcNode>::iterator it1;
			list<ArcNode>::iterator it2;
			list<ArcNode> arcnodes = vertices[i].getArcNodes() ;
			it1 = arcnodes.begin() ;
			bool breakout = false;
			while( it1 != arcnodes.end() ) {
				it1++;
				it2 =it1;
				it1--;
				while( it2 != arcnodes.end() ) {
					/*
					 * (reverse of r2) o ( r1 ) must be universal relation
					 */
					if( lm->composite(lm->getInverse(it2->getConstraint()), it1->getConstraint()).getBitset() != 255) {
						isolate=false;
						breakout = true;
						break;
					}
					
					for(int r=1; r<=128 ; r<<=1) {
						Constraint cnstrt= lm->getConstraint(r);
						Constraint cnstrt_i = lm->getInverse(cnstrt);
						int r1 = it1->getConstraint().getBitset();
						int r2 = it2->getConstraint().getBitset();
						int r1_n = lm->composite( it2->getConstraint(), cnstrt ).getBitset();
						int r2_n = lm->composite( it1->getConstraint(), cnstrt_i ).getBitset();


#if 0
						int r1_n = lm->composite( it2->getConstraint(), cnstrt ).getBitset();
						int r2_n = lm->composite( it1->getConstraint(), cnstrt_i ).getBitset();

						int r1 = it1->getConstraint().getBitset();
						int r2 = it2->getConstraint().getBitset();
#endif
						/*
						 * r1 must be contained in ( r2 o cnstr ) 
						 * r2 must be contained in ( r1 o (inverse of cnstr ))
						 */
						if ( !((r1_n & r1) && ( r2_n & r2)) ) {
							isolate=false;
							breakout=true;
							break;
						}
					}

					if( breakout ) {
						break;
					}
					it2++;
				}
				if(breakout) 
					break;
				it1++;
			}
			if(isolate && deg>10) {
				cout << "del " << i << "deg " << deg << endl;
			}
		} else {
			isolate = false;
		}

		if(!isolate)  {
			new_node_list.insert(new_node_list.end(), vertices[i] ) ;
			mapping[vertices[i].getVid()]=++no;
		} 


	}


	list<RegionNode>::iterator n_it = new_node_list.begin();
	int n_sub = 0;
	while( n_it != new_node_list.end() ) {
		list<ArcNode> arcs = n_it->getArcNodes();
		list<ArcNode>::iterator a_it = arcs.begin();
		while( a_it != arcs.end()) {
			int p_obj = a_it->getAdjvex();
			int n_obj = mapping[ vertices[p_obj].getVid()];
			if( !n_obj) {
				a_it++;
				continue;
			}
			Constraint c = a_it->getConstraint();
			ConstraintArc n_ca( n_sub, n_obj -1 , c);
			n_lca.insert( n_lca.end(), n_ca);
			a_it++;
		}

		n_sub++;
		n_it++;
	}

	n_it = new_node_list.begin();
	while( n_it != new_node_list.end()) {
		n_it->clearArcNodes();
		n_it->setDegZero();
		n_it++;
	}
	ALRCCModel a_rmodel( new_node_list, n_lca, lm);

	return a_rmodel;

}

ALRCCModel
ALRCCModel::delete_isolate_node() const {
	/**
	 * delete the node which indegree == outdegree == 1
	 * segmentation set's size is 1
	 */
	list<RegionNode> new_node_list;
	map<int , int> mapping;
	int no = 0;
	for( int i = 0 ; i < vertices.size(); i++ ) {
		bool isolate = false;
		int indeg = vertices[i].getIndeg();
		int outdeg = vertices[i].getOutdeg();
		if( ((indeg <= 1) && ( outdeg <= 1))) {
			if( (indeg == 0) ||( outdeg == 0) ) {
				isolate = true;
			} else {
				list<ArcNode> nodes = vertices[i].getArcNodes();
			       	list<ArcNode>::iterator it_nodes = nodes.begin();
				int adj = it_nodes->getAdjvex();
				list<ArcNode> nodes_adj = vertices[adj].getArcNodes();
				list<ArcNode>::iterator it_nodes_adj = nodes_adj.begin();
				while( it_nodes_adj != nodes_adj.end() ){
					if( i == it_nodes_adj->getAdjvex()) {
						if( (it_nodes->getConstraint()).getBitset() == \
								(lm->getInverse(it_nodes_adj->getConstraint())).getBitset()) {
							isolate = true;
							break;
						} else {
							cout << i << " " << adj << endl;
						}
					}
					it_nodes_adj++;
				}

			}
		}
		if( !isolate) {
			new_node_list.insert( new_node_list.end(), vertices[i]);
			mapping[vertices[i].getVid()] = ++no;
		}
	}

	
	list<ConstraintArc> n_lca;
	list<RegionNode>::iterator n_it = new_node_list.begin();
	int n_sub = 0;
	while( n_it != new_node_list.end() ) {
		list<ArcNode> arcs = n_it->getArcNodes();
		list<ArcNode>::iterator a_it = arcs.begin();
		while( a_it != arcs.end()) {
			int p_obj = a_it->getAdjvex();
			int n_obj = mapping[ vertices[p_obj].getVid()];
			if( !n_obj) {
				a_it++;
				continue;
			}
			Constraint c = a_it->getConstraint();
			ConstraintArc n_ca( n_sub, n_obj -1 , c);
			n_lca.insert( n_lca.end(), n_ca);
			a_it++;
		}

		n_sub++;
		n_it++;
	}

	n_it = new_node_list.begin();
	while( n_it != new_node_list.end()) {
		n_it->clearArcNodes();
		n_it->setDegZero();
		n_it++;
	}
	ALRCCModel a_rmodel( new_node_list, n_lca, lm);

	return a_rmodel;
}


void 
ALRCCModel::export_to_file( const char * csp_path, const char * map_path ) const{
	ofstream ocsp_file, omap_file;
	ocsp_file.open( csp_path );
	omap_file.open( map_path );
	
	ocsp_file << vexnum-1 << "   #" << csp_path << endl;

	for( int sub = 0 ; sub < vertices.size(); sub++) {
	   list<ArcNode>::iterator it;
	   list<ArcNode>  lan = vertices[sub].getArcNodes();
	   it = lan.begin();
	   while( it != lan.end() ) {
		   int obj = it->getAdjvex();
		   ocsp_file << sub << " " << obj << " ( " << it->getConstraint().getStr() << ")" << endl;  
		   it++;
		   
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


bool
ALRCCModel::setRegions( const list<RegionNode>& lrn ){
	copy(lrn.begin(),lrn.end(),back_inserter(vertices)); 
	return true;
}

bool
ALRCCModel::setConstraints( const list<ConstraintArc>& lca ) {
	list<ConstraintArc>::const_iterator it = lca.begin();
	while(it != lca.end()) {
		int sub = it->getSub();
		int obj = it->getObj();
		Constraint c = it->getConstraint();
		vertices[sub].addOutdeg();
		vertices[obj].addIndeg();
		ArcNode an( obj, c);
		vertices[sub].addArcNode( an );
		it++;
	}
	return true;
}

bool 
ALRCCModel::check_preprocess() const { 
	/*
	 * check if the model satisfies the the preprocess condition
	 * must be used after the function preprocess
	 */
	bool satisfy = true;
	for( int sub = 0 ; sub < vertices.size(); sub++) {
		if(vertices[sub].getIndeg()!=vertices[sub].getOutdeg()) {
			cout << "indeg!=outdeg" << sub << endl;
			satisfy = false;
		}
		list<ArcNode>::iterator it;
		list<ArcNode>  lan = vertices[sub].getArcNodes();
		it = lan.begin();
		while( it != lan.end() ) {
			int obj = it->getAdjvex();
			int relation = it->getConstraint().getBitset() ;
			if( relation == 0 ) {
				satisfy= false;
				cout << "zero relation " << sub << " " << obj << endl;
			}
			list<ArcNode> lan_reverse = vertices[obj].getArcNodes() ;
			list<ArcNode>::iterator it_reverse = lan_reverse.begin() ;
			while(it_reverse != lan_reverse.end() ) {
				if(it_reverse->getAdjvex()==sub ) {
					if (it_reverse->getConstraint().getBitset()==lm->getInverse(lm->getConstraint(relation)).getBitset()) {
						;
					} else {
						cout << "relation not symmetric " << sub << " " << obj << endl;
						satisfy = false;
					}
					break;
				} 
				it_reverse++;
			}
			it++;
		}
	}
	cout << satisfy << endl;
	return satisfy;
}

ALRCCModel
ALRCCModel::preprocess() const {
	list<RegionNode> new_node_list;
	list<ConstraintArc> n_lca;

	for( int sub = 0 ; sub < vertices.size(); sub++) {
	   list<ArcNode>::iterator it;
	   list<ArcNode>  lan = vertices[sub].getArcNodes();
	   it = lan.begin();
	   new_node_list.insert( new_node_list.end() ,vertices[sub] );
	   while( it != lan.end() ) {
		   int obj = it->getAdjvex();
		   int relation = it->getConstraint().getBitset() ;
		   setConstraint( sub, obj, relation , &n_lca );
		   setConstraint( obj, sub, lm->getInverse(lm->getConstraint(relation)).getBitset(), &n_lca );

		   it++;
	   }
	}
	
	list<RegionNode>::iterator n_it;
	n_it = new_node_list.begin();
	while( n_it != new_node_list.end()) {
		n_it->clearArcNodes();
		n_it->setDegZero();
		n_it++;
	}
	ALRCCModel a_rmodel( new_node_list, n_lca, lm);

	return a_rmodel;

}

bool
ALRCCModel::read(const char * csp_path, const char * map_path) {
	ifstream map_file, csp_file;
	map_file.open( map_path);
	csp_file.open( csp_path);
	list<RegionNode> lrn;
	list<ConstraintArc> lca;
	
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
		RegionNode rn( id, name);
		lrn.insert( lrn.end() , rn );
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
		ConstraintArc ca( n1, n2, c);
		
		lca.insert(lca.end(), ca);
	}

	setRegions( lrn );
	setConstraints( lca );
	vexnum = lrn.size();
	arcnum = lca.size();
	

	csp_file.close();
	map_file.close();
	return true;
}

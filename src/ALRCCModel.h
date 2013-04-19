#pragma once

#include <vector>
#include <list>
#include <string>
#include "RCCModel.h" 
#include "LanguageModel.h"


using namespace std;

class ConstraintArc {
	public :
		ConstraintArc( int s, int o, Constraint& c ) {
			sub = s;
			obj = o;
			cnstr = c;
		}

		int getSub() const { return sub;}
		int getObj() const { return obj;}
		Constraint getConstraint() const { return cnstr; }

		void setConstraint( const Constraint cnstr_new ) { cnstr= cnstr_new; }
	private :
		int sub, obj;
		Constraint cnstr;
};


class ArcNode {
	public :
		ArcNode( int adj, Constraint& c ) { adjvex = adj; cnstr = c; }
		int getAdjvex() const { return adjvex ; }
		Constraint getConstraint() const {  return cnstr; }	

	private :
		int adjvex;
		Constraint cnstr;
};


class RegionNode {
	public :
		RegionNode( int id = -1, string n="") { 
			v_id = id; 
			name = n; 
			in_deg = out_deg = 0; 
		}

		int getVid() const { return v_id; }
		int getIndeg() const { return in_deg; }
		int getOutdeg() const { return out_deg; }

		string getName() const { return name;}
		list<ArcNode> getArcNodes() const { return arcnodes; }

		void addIndeg() { ++in_deg ;}
		void addOutdeg() { ++out_deg ; }
		void addArcNode( ArcNode& an ) { arcnodes.insert(arcnodes.end(), an );}
		void setDegZero() { in_deg = out_deg = 0; }
		void clearArcNodes() { arcnodes.clear() ;}
	private :
		int v_id;
		string name;
		int in_deg, out_deg;
		list<ArcNode> arcnodes;
};




class ALRCCModel : public RCCModel{
	public :
		ALRCCModel( const list<RegionNode>& lrn, const list<ConstraintArc>& lca, LanguageModel * l ) { 
			setRegions( lrn );
	       	setConstraints(lca);
			vexnum = lrn.size();
			arcnum = lca.size();
			lm = l;
		}
		ALRCCModel( const char * csp_path, const char * map_path, LanguageModel *l ){
			lm = l;
			read( csp_path, map_path);
		}
		ALRCCModel( const string csp_path, const string map_path, LanguageModel *l ) {
			ALRCCModel( csp_path.c_str(), map_path.c_str(), l );
		}
		
		/**
		 *get function
		 */		
		RegionNode getRegionNode( int n ) { return vertices.at(n) ; }
		RegionNode getRegionNodeById( int ) const ;
		int vexNum() const { return vexnum; }
		int arcNum() const { return arcnum; }
		/**
		 * preprocess
		 */
		ALRCCModel preprocess() const;
		bool check_preprocess() const;
		/**
		 *set regions and constriants
		 */
		bool setRegions( const list<RegionNode>& ) ; 
		bool setConstraints( const list<ConstraintArc>& ); 
		void setConstraint( int , int , int , list<ConstraintArc>*  ) const;
		/*
		 *segmentation function
		 */
		virtual list<ALRCCModel> single_segmentation( const int ) const; 
		virtual ALRCCModel delete_isolate_node( ) const;
		virtual ALRCCModel delete_node_erasable(int  ) const;
		/**
		 * check consistency
		 */
		virtual void check_multi_csp() const;
		virtual void check_inverse_error() const;
		/**
		 * output 
		 */
		void export_to_file( const char *, const char *) const; 
		void export_to_file( const string csp_p, const string map_p ) const {
			export_to_file( csp_p.c_str(), map_p.c_str());
		}
		void print() const;
	private :
		bool read( const char * , const char *);
		list<int> findConnVex( int, int ) const;
		

		vector<RegionNode> vertices;
		int vexnum, arcnum;

		LanguageModel * lm;
};

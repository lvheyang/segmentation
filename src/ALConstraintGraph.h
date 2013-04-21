#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include "Constraint.h"
#include "Vertex.h"
#include "Arc.h"
#include "ConstraintNetwork.h"
#include "Triple.h"
#include "LanguageModel.h"

using namespace std;

class ALConstraintGraph {
	public :
		ALConstraintGraph( ConstraintNetwork *cn, LanguageModel *l );
		
		int getVexnum(void) const { return vexnum; }
		string getVexname( int v ) const { return vertices[v].getName(); }
		int getArcnum(void) const { return arcnum; }

		virtual vector<int> getAdjacent( int m  ) const ;
		virtual vector<int> getAdjacent( Vertex m  ) const { return getAdjacent( m.getVid()); }
		virtual vector<Arc> getAdjacentArcs( int i  ) const { return vertices[i].getArcs();}
		
		/**
		 * 获得与点m包含rel关系的所有邻接点
		 */
		virtual vector<int> getAdjacentBy( int m, int rel ) const ;
		virtual vector<int> getAdjacentBy( int m, string rel ) const {
			return getAdjacentBy(m, lm->getConstraint(rel).getBitset());
		}
		virtual vector<int> getAdjacentBy( int m, Constraint rel ) const {
			return getAdjacentBy(m, rel.getBitset() );
		}


		virtual void addEdge( Triple t ) ;
		virtual void delEdge( int m , int  n );

		vector<string> toCSPString( int no ) const;
		vector<string> toMAPString( int no ) const;
		void exportToFile( const char * csppath, const char * mappath ) const ;
		void exportToFile( string csppath, string mappath ) const {
			exportToFile( csppath.c_str() , mappath.c_str());
		}
	private :
		int vexnum ;
		int arcnum ;
		vector<Vertex> vertices ;
		LanguageModel *lm;
};

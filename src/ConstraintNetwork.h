#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include "Triple.h"
#include "Constraint.h"
#include "LanguageModel.h"
#include "util.h"

using namespace std;

class ConstraintNetwork {
	public :
		ConstraintNetwork( LanguageModel *l ) {
			vexnum = 0;
			lm = l;
		}
		
		ConstraintNetwork( int v, int e, LanguageModel *l ) ;

		ConstraintNetwork( const string csppath, const string mappath, LanguageModel *l ) {
			/**
			 * @bug
			 */
			ConstraintNetwork( csppath.c_str(), mappath.c_str() ,l  );
		}

		ConstraintNetwork( const char * csppath, const char * mappath, LanguageModel *l  ) {
			vexnum = 0;
			lm = l;
			read( csppath, mappath);
		}
		
		/**
		 * 按照顺序加入节点
		 */
		void addVertex( string name ) ;
		void addVertices( vector<string> name_set ) ;
		void addConstraint( string name1, string name2, Constraint c );
		void addConstraint( int v, int w, Constraint c );

		/**
		 * 获得节点的编号
		 */
		int getVindex( string name ) const ;
		string getNameAt( int index ) const { return names.at( index ); }
		int getNamenum( void) const {return names.size();}

		Triple getTripleAt( int index ) const { return triples.at( index ); }
		int getTripleNum( void ) const { return triples.size(); }

		/**
		 * 输出
		 */
		bool exportToFile( const char * csppath, const char * mappath) const ;
		bool exportToFile( const string csppath, const string mappath ) const {
			exportToFile( csppath.c_str(), mappath.c_str());
		}

	private :
		int vexnum;
		LanguageModel *lm;
		vector<Triple> triples;
		vector<string> names;

		/**
		 * 二分查找 返回查找得到的位置(应插入位置)
		 */
		int binsearch( string name ) const ;

		/**
		 * 从文件中读取
		 */
		bool read( const char * csppath, const char * mappath ) ;
};

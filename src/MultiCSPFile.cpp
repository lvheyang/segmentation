

#include "MultiCSPFile.h"


//#define DBG

#ifdef DBG
#include <iostream>
#endif

void
MultiCSPFile::exportToFile( const char * csppath ,const char * mappath ) const {
	ofstream csp_output;
	ofstream map_output;
	csp_output.open( csppath);
	map_output.open( mappath);
	list<ALConstraintGraph>::iterator it ;
	it = lptr->begin();
	int no = 1;	
	while( it!=lptr->end() ) {
		vector<string> cstrings = it->toCSPString(no);
		vector<string> mstrings = it->toMAPString(no);
		string s ;
		for(int i = 0; i < cstrings.size(); i++ ) {
			s = cstrings[i];
#ifdef DBG
			cout  << s;
#endif
			csp_output << s ;
		}
        for( int i = 0 ; i < mstrings.size(); i++ ) {
            s= mstrings[i];
            map_output << s;
        }
		it++;
		no++;
	}
}

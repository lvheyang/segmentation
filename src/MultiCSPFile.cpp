

#include "MultiCSPFile.h"


//#define DBG

#ifdef DBG
#include <iostream>
#endif

void
MultiCSPFile::exportToFile( const char * csppath ) const {
	ofstream output;
	output.open( csppath);
	list<ALConstraintGraph>::iterator it ;
	it = lptr->begin();
	int no = 1;	
	while( it!=lptr->end() ) {
		vector<string> strings = it->toCSPString(no);
		string s ;
		for(int i = 0; i < strings.size(); i++ ) {
			s = strings[i];
#ifdef DBG
			cout  << s;
#endif
			output << s ;
		}
		it++;
		no++;
	}
}

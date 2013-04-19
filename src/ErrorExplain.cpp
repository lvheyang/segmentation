#pragma once

#include <string>
#include "Explain.h"
#include "AssertionBase.h"
#include "ErrorExplain.h"

using namespace std;

list<Explain>
ErrorExplain::getAllExpls(const string sub, const string obj) {
	list<Explain> l;
	
	base->begin();
	while( base->hasNxtExpl()) {
		Explain e = base->getNxtExpl();
		if( (sub == e.getSubject() && obj == e.getObject() ) 
				|| ( sub == e.getObject() && obj==e.getSubject()))
			l.insert(l.end(), e);
	}
	return l;
}

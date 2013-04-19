#pragma once

#include <string>

using namespace std;

class Explain {
	public :
		Explain(string sub, string obj, string pro) :
			subject(sub), object(obj), property(pro) {};
		string getSubject() const {return subject;}
		string getObject() const {return object;}
		string getProperty() const {return property;}
		
	
	private :
		string subject;
		string object;
		string property;
};

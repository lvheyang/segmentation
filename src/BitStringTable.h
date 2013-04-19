#pragma once

#include <string>

using namespace std;


class BitStringTable {
	public :
		virtual int getBit(const string ) const = 0;
		virtual string getString(const int ) const = 0;
		virtual ~BitStringTable() {}
		virtual int getBase() const = 0;
};


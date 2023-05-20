#include "type.h"
using namespace std;

void Field::dumpInner(const int i) const{
	indent(i);
	cout << *type << endl;
	indent(i);
	cout << *name << endl;
}

void Field::dump(const int i) const {
	dumpOuter("Field", i);
}


void SimpleType::dumpInner(const int i) const {
	indent(i);
	cout << *name << endl;
}
void SimpleType::dump(const int i) const {
	dumpOuter("SimpleType", i);
}
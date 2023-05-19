#include "fun.h"
using namespace std;

void FunDef::dumpInner(const int i) const {
	type->dump(i);
	indent(i);
	cout << *name << endl;
	for(auto &f : *fields) 
		f->dump(i);
	block->dump(i);
}
void FunDef::dump(const int i) const {
	dumpOuter("FunDef", i);
}

void CompUnit::dumpInner(const int i) const {
	func_def->dump(i);
}
void CompUnit::dump(const int i) const {
	dumpOuter("CompUnit", i);
}
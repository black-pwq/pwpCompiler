#include "ast.h"
using namespace std;

void BaseAST::dump(const int i) const {
	indent(i);
	std::cout << abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, NULL)
	<< " (#line " << lineno << ") " << "{" << std::endl;
	dumpInner(i + 1);
	indent(i);
	std::cout << "}" << std::endl;
}

void BaseAST::dumpInner(const int i) const {
	indent(i);
	std::cout << "NONPRINTABLE" << std::endl;
}

void BaseAST::indent(const int i) const {
	for(int j = 0; j < i; j++)
		std::cout << "  ";
}

void FunDef::dumpInner(const int i) const {
	type->dump(i);
	indent(i);
	cout << *name << endl;
	for(auto &f : *fields) 
		f->dump(i);
	block->dump(i);
}

void Return::dumpInner(const int i) const {
	expr->dump(i);
}

void Block::dumpInner(const int i) const {
	for(auto &s : items) {
		s->dump(i);
	}
}

void CompUnit::dumpInner(const int i) const {
	for(auto &u : units)
		u->dump(i);
}

void Field::dumpInner(const int i) const{
	type->dump(i);
	name->dump(i);
}

void Type::dumpInner(const int i) const {
	indent(i);
	cout << *name << endl;
}
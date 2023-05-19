#include "stmt.h"
using namespace std;

void RetStmt::dumpInner(const int i) const {
	expr->dump(i);
}
void RetStmt::dump(const int i) const{
	dumpOuter("RetStmt", i);
}

void BlockStmt::dumpInner(const int i) const {
	for(auto &s : stmts) {
		s->dump(i);
	}
}
void BlockStmt::dump(const int i) const{
	dumpOuter("BlockStmt", i);
}
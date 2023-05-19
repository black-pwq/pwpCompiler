#include "expr.h"
using namespace std;

void IntExpr::dump(const int i) const {
	dumpOuter("IntExpr", i);
}
void IntExpr::dumpInner(const int i) const {
	indent(i);
	std::cout << num << std::endl;
}
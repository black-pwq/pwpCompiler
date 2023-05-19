#include "base.h"

void BaseAST::dumpOuter(std::string name, const int i) const {
	indent(i);
	std::cout << name << " {" << std::endl;
	dumpInner(i + 1);
	indent(i);
	std::cout << "}" << std::endl;
}
void BaseAST::indent(const int i) const {
	for(int j = 0; j < i; j++)
		std::cout << "  ";
}
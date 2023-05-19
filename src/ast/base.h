#pragma once
#include <memory>
#include <vector>
#include <iostream>

struct BaseAST
{
	virtual ~BaseAST() = default;
	virtual void dump(const int i = 0) const = 0;
	void dumpOuter(std::string name, const int i) const;
	void indent(const int i) const;
	virtual void dumpInner(const int i) const = 0;
};

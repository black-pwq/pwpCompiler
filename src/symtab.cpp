#include "symtab.h"
#include <iostream>

using namespace std;

SymScope::SymScope() : parent(nullptr), scopeSize(0) {}

SymScope::SymScope(SymScope *p) : parent(p), scopeSize(0)
{
	if (parent != nullptr)
		parent->addChild(this);
}

SymScope::~SymScope()
{
	// delete symbols (value)
	for (auto &pair : scopeTable)
		delete pair.second;
	for (auto scope : children)
		delete scope;
}

void SymScope::dump(FILE *f, int nest_level)
{
	// Recursively prints out the symbol table
	// from the head down through all the childrens
	for (int i = 0; i < nest_level; i++)
		fprintf(f, "\t");
	fprintf(f, "+-- Symbol Scope %d ---\n", scopeSize);

	for (auto &si : scopeTable)
	{
		for (int i = 0; i < nest_level; i++)
			fprintf(f, "\t");
		fprintf(f, "| %s \t", (char *)si.first.c_str());
		si.second->dump(f);
		fprintf(f, "\n");
	}

	for (int i = 0; i < nest_level; i++)
		fprintf(f, "\t");
	fprintf(f, "+-------------\n\n");

	// Now print all the children
	for (auto child : children)
		child->dump(f, nest_level + 1);
}

void SymScope::addChild(SymScope *c)
{
	children.push_back(c);
}

SymScope *SymScope::openScope()
{
	return new SymScope(this);
}

SymScope *SymScope::closeScope()
{
	return parent;
}

bool SymScope::exist(const std::string &name)
{
	Symbol *s = lookup(name);
	// Return true if name exists
	if (s != nullptr)
		return true;
	else
		return false;
}

Symbol *SymScope::insert(const string &name, Symbol *s)
{
	auto pair = scopeTable.insert({name, s});
	if (pair.second)
	{
		s->offset = scopeSize;
		scopeSize += s->getSize();
		s->symScope = this;
		return s;
	}
	else // dup entry
		return pair.first->second;
}

Symbol *SymScope::lookup(const string &name)
{
	// First check the current table;
	ScopeTableType::const_iterator i;
	i = scopeTable.find(name);
	if (i != scopeTable.end())
		return i->second;

	// Failing that, check all the parents;
	if (parent != nullptr)
		return parent->lookup(name);
	else
		// If this has no parents, then it cannot be found
		return nullptr;
}

SymTab::SymTab()
{
	head = new SymScope;
	currScope = head;
}

SymTab::~SymTab()
{
	delete head;
}

void SymTab::openScope()
{
	currScope = currScope->openScope();
	assert(currScope != nullptr);
}

void SymTab::closeScope()
{
	// check to make sure we don't pop more than we push
	assert(currScope != head);
	assert(currScope != nullptr);

	currScope = currScope->closeScope();
}

SymScope *SymTab::getCurrScope()
{
	// check that we actually have a scope before we return it
	assert(currScope != nullptr);
	return currScope;
}

bool SymTab::exist(const string &name)
{
	return currScope->exist(name);
}

Symbol *SymTab::insert(const string &name, Symbol *s)
{
	assert(s != nullptr);
	return currScope->insert(name, s);
}

Symbol *SymTab::insertInParent(const string &name, Symbol *s)
{
	assert(s != nullptr);
	assert(currScope->parent != nullptr);
	return currScope->parent->insert(name, s);
}

Symbol *SymTab::lookup(const string &name)
{
	return currScope->lookup(name);
}

Symbol *SymTab::lookup(SymScope *targetscope, const string &name)
{
	assert(targetscope != nullptr);
	return targetscope->lookup(name);
}

int SymTab::scopeSize(SymScope *targetscope)
{
	return targetscope->scopeSize;
}

int SymTab::distance(SymScope *higher_scope, SymScope *deeper_scope)
{
	assert(deeper_scope != nullptr);
	assert(higher_scope != nullptr);
	if (higher_scope == deeper_scope)
	{
		return 0;
	}
	return distance(higher_scope, deeper_scope->parent) + 1;
}

void SymTab::dump(FILE *f)
{
	head->dump(f, 0);
}
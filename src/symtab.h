#include "symbol.h"
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <list>

typedef std::unordered_map<std::string, Symbol *> ScopeTableType;

class SymScope
{
private:
	SymScope *parent;
	std::list<SymScope *> children;
	ScopeTableType scopeTable;
	int scopeSize;
	SymScope *getParent();
	void addChild(SymScope *c);
	SymScope(SymScope *parent);

	void dump(FILE *f, int nest_level);
	bool exist(const std::string &name);

	/**
	 * Return the pointer to the newly created child scope
	*/
	SymScope *openScope();

	/**
	 * Return the pointer to the parent scope, 
	 * the sub-scope will be stored at the children list 
	 * in the current scope
	*/
	SymScope *closeScope();

	/**
	 * return the successfully inserted Symbol or
	 * the existing one in the current scope
	 */
	Symbol *insert(const std::string &name, Symbol *s);

	/**
	 * return the exisiting Symbol in the
	 * current and parent scopes, otherwise
	 * return nullptr
	 */
	Symbol *lookup(const std::string &name);

public:
	SymScope();
	~SymScope();

	friend class SymTab;
};

class SymTab
{
private:
	SymScope *head;
	SymScope *currScope;

public:
	SymTab();
	~SymTab();

	/**
	 * Create a new child scope from current scope and 
	 * make the current scope points to the new one.
	 * Automatically link the child-parent relationship.
	*/
	void openScope();

	/**
	 * Make the current scope points to the parent.
	 * the sub-scope will be stored at the children list 
	 * in the current scope
	*/
	void closeScope();

	SymScope *getCurrScope();

	// Returns true if name is found in the current SymTab or any of the parents
	bool exist(const std::string &name);

	/**
	 * return the successfully inserted Symbol or
	 * the existing one in the current scope
	 */
	Symbol *insert(const std::string &name, Symbol *s);

	/**
	 * return the successfully inserted Symbol or
	 * the existing one in the current scope
	 */
	Symbol *insertInParent(const std::string &name, Symbol *s);

	/**
	 * return the exisiting Symbol in the
	 * current and parent scopes, otherwise
	 * return nullptr
	 */
	Symbol *lookup(const std::string &name);

	/**
	 * return the exisiting Symbol in the
	 * current and parent scopes, otherwise
	 * return nullptr
	 */
	static Symbol *lookup(SymScope *targetscope, const std::string &name);

	// Returns the size of the targetscope (in bytes) in terms of the total
	// amount of space that would be required to store all the variables in
	// that scope.
	static int scopeSize(SymScope *targetscope);

	// Returns the lexical distantance between deeper_scope and higher_scope.
	// the lexical depth is the levels of nesting between the two (if there
	// are in the same nest then the distance is 0).  If deeper_scope is not
	// _nested_inside_ the higher_scope as assertion will fail
	int distance(SymScope *higher_scope, SymScope *deeper_scope);

	void dump(FILE *f);
};
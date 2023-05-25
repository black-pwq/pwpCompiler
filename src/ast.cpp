#include "ast.h"

using namespace std;





llvm::Value *Return::codegen()
{
    return nullptr;
}

llvm::Value *For::codegen()
{
    return nullptr;
}




llvm::Value *While::codegen()
{
	// llvm::BasicBlock* for_cont = llvm::BasicBlock::Create(*TheContext, "for.cond", F);
	// llvm::BasicBlock* for_body = llvm::BasicBlock::Create(*TheContext, "for.body", F);
	// llvm::BasicBlock* for_end = llvm::BasicBlock::Create(*TheContext, "for.end", F);
	// Builder->CreateBr(for_cont);
    return nullptr;
}



llvm::Value *IfElse::codegen()
{

    return nullptr;
}



llvm::Value *If::codegen()
{
	
    return nullptr;
}






llvm::Value *Block::codegen()
{
    return nullptr;
}








int Expr::t;

llvm::Value *VarDecl::codegen()
{
	for (int i = 0;i < vardefs->size();i++){
		(*vardefs)[i]->valuedef(type->btype);

	}
    return nullptr;
}

static float eval_bi(int l, BiOp op, int r)
{
	switch (op)
	{
	case BiOp::bi_add:
		return l + r;
	case BiOp::bi_sub:
		return l - r;
	case BiOp::bi_times:
		return l * r;
	case BiOp::bi_divide:
		return l / r;
	case BiOp::bi_eq:
		return l == r;
	case BiOp::bi_neq:
		return l != r;
	case BiOp::bi_lt:
		return l < r;
	case BiOp::bi_gt:
		return l > r;
	case BiOp::bi_le:
		return l <= r;
	case BiOp::bi_ge:
		return l >= r;
	case BiOp::bi_or:
		return l || r;
	case BiOp::bi_and:
		return l && r;
	default:
		assert(false);
	}
}

BiExpr::BiExpr(Expr *l, BiOp o, Expr *r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}
BiExpr::BiExpr(std::shared_ptr<Expr> l, BiOp o, Expr *r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}
BiExpr::BiExpr(Expr *l, BiOp o, std::shared_ptr<Expr> r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}
BiExpr::BiExpr(std::shared_ptr<Expr> l, BiOp o, std::shared_ptr<Expr> r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}

UniExpr::UniExpr(UniOp o, Expr *e) : op(o), expr(e)
{
	evaluable = expr->evaluable;
	if (evaluable)
		switch (op)
		{
		case UniOp::uni_minus:
			num = -expr->num;
			break;
		case UniOp::uni_not:
			num = !expr->num;
			break;
		case UniOp::uni_plus:
			break;
		default:
			assert(false);
		}
}

void BaseAST::dump(const int i) const
{
	indent(i);
	const char *name = abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, NULL);
	std::cout << name
			  << " (#line " << lineno << ") "
			  << "{" << std::endl;
	dumpInner(i + 1);
	indent(i);
	std::cout << "}    $" << name << std::endl;
}

void BaseAST::dumpInner(const int i) const
{
	indent(i);
	std::cout << "NONPRINTABLE" << std::endl;
}

void BaseAST::indent(const int i) const
{
	for (int j = 0; j < i; j++)
		std::cout << "  ";
}

void FunDef::unitcodegen()
{

	std::vector<llvm::Type*> field;

	if(fields->size()>0){

	
		for (int i = 0; i<fields->size(); ++i)
		{
			BType arty = ((*fields)[i]->type->btype);
			int dim = ((*fields)[i]->var->nameSym->symbol->wordsInDim.size());
			std::cout << dim<<std::endl;
			if(dim == 0){
				switch (arty)
				{
				case bt_int:
					field.push_back(llvm::Type::getInt32Ty(*TheContext));
					break;
				case bt_float:
					field.push_back(llvm::Type::getFloatTy(*TheContext));
					break;
				default:
					break;
				}
			}
			else if(dim == 1){
				field.push_back(llvm::ArrayType::get(llvm::Type::getInt32Ty(*TheContext),(*fields)[i]->var->nameSym->symbol->wordsInDim[0]));

			} 
			// else if(((*fields)[i]->type->btype)==bt_int ){

			// }

		}

	}
			// field.push_back(llvm::ArrayType::get(llvm::Type::getInt32Ty(*TheContext),10000));
		// field.push_back(llvm::Type::getInt32Ty(*TheContext));
		// field.push_back(llvm::Type::getInt32Ty(*TheContext));
	llvm::FunctionType *ft;

	if((type->btype) == bt_int){
		std::cout<<"int func" <<std::endl;
		ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext),field,false);
	}
	else if((type->btype) == bt_float){
		ft = llvm::FunctionType::get(llvm::Type::getFloatTy(*TheContext),field,false);

	}
	else if((type->btype) == bt_char){
		std::cout<<"return char no support"<<std::endl;

	}else if((type->btype) == bt_void){
		ft = llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext),field,false);
	}
	else{
		std::cout<<"fundef fault"<<std::endl;
	}
	

	// llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(getGlobalContext()),Doubles, false);
  	llvm::Function *F = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, *nameSym->name, TheModule.get());
    
	for (int i = 0;i < fields->size();i++){
			F->getArg(i)->setName(*(*fields)[i]->var->nameSym->name);

	}
			// F->getArg(1)->setName("b");

	llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
  	Builder->SetInsertPoint(BB);
	for (int i = 0;i < block->items.size();i++){
		*block->items[i]->codegen();
	}

	
	if(*nameSym->name == "main"){
		Builder->CreateRet(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*TheContext),0));

	}else
		Builder->CreateRetVoid();
    verifyFunction(*F);
}




void FunDef::dumpInner(const int i) const
{
    type->dump(i);
	indent(i);
	cout << *nameSym->name << endl;
	for (auto &f : *fields)
		f->dump(i);
	block->dump(i);
}

void Return::dumpInner(const int i) const
{
	expr->dump(i);
}

void Block::dumpInner(const int i) const
{
	for (auto &s : items)
		s->dump(i);
}

void CompUnit::unitcodegen()
{
	// initmodule();
	std::cout<<"compunit count " << units.size() <<std::endl;
	for (int i = 0; i<units.size(); i++)
	{
 
		units[i]->unitcodegen();
	}
}

void CompUnit::dumpInner(const int i) const
{
	for (auto &u : units)
		u->dump(i);
}

void Field::dumpInner(const int i) const
{
	type->dump(i);
	var->dump(i);
}

void VarDecl::dumpInner(const int i) const
{
	type->dump(i);
	for (auto &vardef : *vardefs)
	{
		vardef->dump(i);
	}
}


void VarDef::dumpInner(const int i) const
{
	var->dump(i);
}


void Var::dumpInner(const int i) const
{
	indent(i);
	cout << *nameSym->name << endl;
}

void ArrayVar::dumpInner(const int i) const
{
	indent(i);
	cout << *nameSym->name << endl;
	for (auto &e : exprs->list)
	{
		e->dump(i);
	}
}

void Expr::dumpInner(const int i) const
{
	indent(i);
	cout << "eval: " << evaluable << endl;
}

void UniExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	char n[] = {'+', '-', '!'};
	indent(i);
	cout << n[op] << endl;
	expr->dump(i);
}

llvm::Value *UniExpr::codegen()
{
	llvm::Value* opnd = expr->codegen();
	if(op == uni_plus){
		return opnd;
	}
	else if(op == uni_minus){
		
		// return Builder->create;
	}
	else if(op == uni_not){

	}
	
    return nullptr;
}

llvm::Value *BiExpr::codegen()
{

	llvm::Value *L = left->codegen();
	llvm::Value *R = right->codegen();

	auto typeL = L->getType();
	auto typeR = R->getType();
	if (!L || !R)
		return nullptr;
	// if(typeL == llvm::Type::DoubleTyID)
	std::cout << "succ bin"<<std::endl;
	switch (op)
	{
		case bi_add:
			return Builder->CreateAdd(L, R, "addtmp");
		case bi_sub:
			return Builder->CreateSub(L, R, "subtmp");
		case bi_times:
			return Builder->CreateMul(L, R, "multmp");
		case bi_divide:
			return nullptr;
		case bi_lt:
			return Builder->CreateICmpSLT(L, R, "cmptmplt");
		case bi_le:
			return Builder->CreateICmpSLE(L, R, "cmptmple");
		case bi_gt:
			return Builder->CreateICmpSLT(L, R, "cmptmpgt");
		case bi_ge:
			return Builder->CreateICmpSLT(L, R, "cmptmpge");
		case bi_eq:
			return Builder->CreateICmpEQ(L, R, "cmptmpeq");
		case bi_neq:
			return Builder->CreateICmpNE(L, R, "cmptmpneq");
		case bi_and:
			return Builder->CreateAnd(L, R, "cmptmpand");
		case bi_or:
			return Builder->CreateOr(L, R, "cmptmpor");
		default:
			return nullptr;
	}

	
    return nullptr;
}

llvm::Value *Call::codegen()
{
	std::vector<llvm::Type *> inArgs;

    inArgs.push_back(Builder->getVoidTy());
    llvm::ArrayRef<llvm::Type*>  argsRef2(inArgs);
	//scanf def
    llvm::FunctionType *inType =llvm::FunctionType::get(Builder->getInt32Ty(), argsRef2, false);
    llvm::Function *inFunc = llvm::dyn_cast<llvm::Function>(TheModule->getOrInsertFunction(*name, inType).getCallee());
	
	
    return nullptr;
}

llvm::Value *Expr::codegen()
{
	
    return nullptr;
}

llvm::Value *ExprList::codegen()
{
	for (int i =0 ;i < list.size();i++){
		list[i]->codegen();
	}
	return nullptr;
}

void BiExpr::dumpInner(const int i) const
{
    
	Expr::dumpInner(i);
	const std::string opNames[] = {"+", "-", "*", "/", "&&", "||", "==", "!=", "<", "<=", ">", ">="};
	left->dump(i);
	indent(i);
	std::cout << opNames[op] << " opcode = " << op << std::endl;
	right->dump(i);
}

void StringExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	indent(i);
	cout << *str << endl;
}

void IntExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	indent(i);
	cout << num << endl;
}

void FloatExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	indent(i);
	cout << num << endl;
}

void Type::dumpInner(const int i) const
{
	const std::string t[] = {"undef", "int", "float", "void"};
	indent(i);
	std::cout << t[btype] << std::endl;
}

llvm::Value *Continue::codegen()
{
    return nullptr;
}

llvm::Value *Break::codegen()
{
    return nullptr;
}

llvm::Value *Stmt::codegen()
{
    return nullptr;
}

void Unit::unitcodegen()
{
	
}
void InitVarDef::dumpInner(const int i) const
{
	var->dump(i);
	expr->dump(i);
}

llvm::Value * SimpleVar::codegen()
{
	if(nameSym->symbol->type == bt_int){
		if(evaluable == 1){
			auto ty = llvm::IntegerType::getInt32Ty(*TheContext);
			llvm::Constant *i32_val = llvm::ConstantInt::get(ty,(int)(num), true);
			return i32_val;
		} 
		else{
			auto ty = llvm::IntegerType::getInt32Ty(*TheContext);
			// TheContext
			llvm::BasicBlock* bb = Builder->GetInsertBlock();
			auto stable = bb->getValueSymbolTable();
			auto fin = stable->lookup(*nameSym->name);
			return Builder->CreateLoad(llvm::IntegerType::getInt32Ty(*TheContext),fin,"itmp");
			// TheContext
    		

		}
	}
	else if(nameSym->symbol->type == bt_int){

	}
	else if(nameSym->symbol->type == bt_float){
		
	}
	return nullptr;
}


llvm::Value *ArrayVar::codegen()
{
    return nullptr;
}

void InitArrayDef::valuedef(BType bt)
{
	
}
void InitVarDef::valuedef(BType bt)
{
	var->vardefwithoutinit(bt);
	llvm::BasicBlock* bb = Builder->GetInsertBlock();
	auto stable = bb->getValueSymbolTable();
	auto fin = stable->lookup(*var->nameSym->name);
	auto vars = Builder->CreateLoad(llvm::IntegerType::getInt32Ty(*TheContext),fin,"itmp");
	auto value = expr->codegen();
	Builder->CreateStore(value,vars);

}

void VarDef::valuedef(BType bt)
{
	var->vardefwithoutinit(bt);
}


void SimpleVar::vardefwithoutinit(BType bt)
{
	switch (bt)
	{
	case bt_int:
		Builder->CreateAlloca(llvm::IntegerType::get(TheModule->getContext(), 32), NULL, *nameSym->name);

		break;
	
	case bt_float:
		Builder->CreateAlloca(llvm::Type::getFloatTy(TheModule->getContext()), NULL, *nameSym->name);
		break;
	

	default:
		break;
	}
}

void ArrayVar::vardefwithoutinit(BType bt)
{
	switch (bt)
	{
	case bt_int:
		// Builder->CreateAlloca(llvm::IntegerType::get(TheModule->getContext(), 32), NULL, *nameSym->name);

		break;
	
	case bt_float:
		// Builder->CreateAlloca(llvm::Type::getFloatTy(TheModule->getContext()), NULL, *nameSym->name);
		break;
	

	default:
		break;
	}
}

llvm::Value *Assign::codegen()
{
	
	auto vars = var->codegen();
	auto value = expr->codegen();
	return Builder->CreateStore(value,vars);
    
}

llvm::Value *IntExpr::codegen()
{

	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), (int)(num));
    return nullptr;
}

llvm::Value *FloatExpr::codegen()
{
	return  llvm::ConstantFP::get(*TheContext, llvm::APFloat(num));
    return nullptr;
}
void Call::dumpInner(const int i) const {
	indent(i);
	cout << *name << endl;
	params->Expr::dump(i);
}

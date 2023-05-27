%code requires {
  #include <memory>
  #include "ast.h"
  #include <string>
}

%{

#include "ast.h"
#include <iostream>
#include <memory>
#include <string>

extern int yylineno;
int yylex();
void yyerror(std::unique_ptr<CompUnit> &ast, const char *s);

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<CompUnit> &ast }

%union {
  std::string *str_val;
  int int_val;
  float float_val;
  BaseAST *ast_val;
  CompUnit *comp_val;
  FunDef *fundef_val;
  Stmt *stmt_val;
  Type *type_val;
  Expr *expr_val;
  Block *block_val;
  Var *var_val;
  VarDefs *vardef_val;
  VarDecl *var_decl_val;
  Decl *decl_val;
  ExprList *exprlist_val;
  FieldList *fieldlist_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN FLOAT VOID BREAK CONTINUE IF ELSE WHILE LE GE EQ NEQ AND OR FOR CONST PASSIGN MASSIGN TASSIGN DASSIGN
%token <str_val> IDENT STRING
%token <int_val> INT_CONST
%token <float_val> FLOAT_CONST

// 非终结符的类型定义
/* %type FunDef Block Stmt Number VarDecl VarDef Decl */
/* %type <comp_val> CompUnit */
%type <fundef_val> FunDef
%type <expr_val> T F E R Eq C Cond And Expr
%type <block_val> Block BlockItems
%type <stmt_val> Stmt
%type <vardef_val> VarDef
%type <type_val> BType
%type <var_val> Var FunFPVar
%type <var_decl_val> VarDecl
%type <decl_val> Decl
%type <exprlist_val> ExprList ArrayInitVal FunRP SquareExprList
%type <fieldlist_val> FunFP

%%

// { auto comp_unit = make_unique<CompUnit>($1); ast = move(comp_unit); }
CompUnit
  : FunDef  {ast = make_unique<CompUnit>(CompUnit($1));}
  | Decl    {ast = make_unique<CompUnit>(CompUnit($1));}
  | CompUnit FunDef   {ast->append($2);}
  | CompUnit Decl     {ast->append($2);}
  ;

Decl
  : VarDecl   {$$ = $1;}
  ;

VarDecl
  : BType VarDef ';'  {$$ = new VarDecl($1, $2);}
  ;

BType
  : INT   {$$ = new Type(BType::bt_int);}
  | FLOAT {$$ = new Type(BType::bt_float);}
  ;

VarDef
  : Var                             {auto v = new VarDefs(); v->emplace_back(unique_ptr<VarDef>(new VarDef($1))); $$ = v;}
  | Var '=' Expr                       {auto v = new VarDefs(); v->emplace_back(unique_ptr<InitVarDef>(new InitVarDef($1, $3))); $$ = v;}
  | Var '=' ArrayInitVal            {auto v = new VarDefs(); v->emplace_back(unique_ptr<InitArrayDef>(new InitArrayDef($1, $3))); $$ = v;}
  | VarDef ',' Var                  {$1->emplace_back(unique_ptr<VarDef>(new VarDef($3))); $$ = $1;}
  | VarDef ',' Var '=' Expr            {$1->emplace_back(unique_ptr<InitVarDef>(new InitVarDef($3, $5))); $$ = $1;}
  | VarDef ',' Var '=' ArrayInitVal {$1->emplace_back(unique_ptr<InitArrayDef>(new InitArrayDef($3, $5))); $$ = $1;}
  ;

/* ArrayList
  : '{' ExprList '}'
  | ArrayList ',' '{' ExprList '}'
  ; */

ArrayInitVal
  : '{' ExprList '}'    {$$ = $2;}
  ;

ExprList
  : Expr                   {$$ = new ExprList($1);}
  | ExprList ',' Expr      {$1->append($3); $$ = $1;}
  ;

SquareExprList
  : '[' Expr ']'                {$$ = new ExprList($2);}
  | SquareExprList '[' Expr ']' {$1->append($3); $$ = $1;}
  ;

Var 
  : IDENT                     {$$ = new SimpleVar($1);}
  | IDENT SquareExprList      {$$ = new ArrayVar($1, $2);} 
  ;

F
	: INT_CONST   {$$ = new IntExpr($1);}
  | FLOAT_CONST {$$ = new FloatExpr($1);}
  | IDENT '(' FunRP ')'  {$$ = new Call($1, $3);}
  | Var                     {$$ = $1;}
	| '(' Expr ')'		        {$$ = $2;}
	;

R
	: '-' F				{$$ = new UniExpr(UniOp::uni_minus, $2);}
	| '+' F				{$$ = new UniExpr(UniOp::uni_plus, $2);}
	| '!' F				{$$ = new UniExpr(UniOp::uni_not, $2);}
	| F           {$$ = $1;}
	;

T
  : T '*' R 		{$$ = new BiExpr($1, BiOp::bi_times, $3);}
	| T '/' R 		{$$ = new BiExpr($1, BiOp::bi_divide, $3);}	
	| R           {$$ = $1;}
	;

E
	: E '+' T 		{$$ = new BiExpr($1, BiOp::bi_add, $3);}
	| E '-' T 		{$$ = new BiExpr($1, BiOp::bi_sub, $3);}	
	| T           {$$ = $1;}
	;

C
  : E           {$$ = $1;}
  | C '<' E     {$$ = new BiExpr($1, BiOp::bi_lt, $3);}
  | C '>' E     {$$ = new BiExpr($1, BiOp::bi_gt, $3);} 
  | C LE E      {$$ = new BiExpr($1, BiOp::bi_le, $3);}  
  | C GE E      {$$ = new BiExpr($1, BiOp::bi_ge, $3);}
  ;

Eq
  : C           {$$ = $1;}
  | Eq EQ C     {$$ = new BiExpr($1, BiOp::bi_eq, $3);}
  | Eq NEQ C    {$$ = new BiExpr($1, BiOp::bi_neq, $3);}
  ;

And
  : Eq          {$$ = $1;}
  | Eq AND Eq   {$$ = new BiExpr($1, BiOp::bi_and, $3);}
  ;

Cond
  : And         {$$ = $1;}
  | Cond OR And {$$ = new BiExpr($1, BiOp::bi_or, $3);}
  ;

Expr 
  : Cond        {$$ = $1;}
  | STRING      {$$ = new StringExpr($1);}
  | Var '=' Cond{$$ = new Assign($1, $3);}
  | Var PASSIGN Cond  {auto v = shared_ptr<Var>($1); $$ = new Assign(v, new BiExpr(v, BiOp::bi_add, $3));}
  | Var MASSIGN Cond  {auto v = shared_ptr<Var>($1); $$ = new Assign(v, new BiExpr(v, BiOp::bi_sub, $3));}
  | Var TASSIGN Cond  {auto v = shared_ptr<Var>($1); $$ = new Assign(v, new BiExpr(v, BiOp::bi_times, $3));}
  | Var DASSIGN Cond  {auto v = shared_ptr<Var>($1); $$ = new Assign(v, new BiExpr(v, BiOp::bi_divide, $3));}
  ;

FunDef
  : BType IDENT '(' FunFP ')' Block {$$ = new FunDef($1, $2, $4, $6);}
  | VOID IDENT '(' FunFP ')' Block  {$$ = new FunDef(new Type(BType::bt_void), $2, $4, $6);}
  ;

FunFPVar
  : Var   {$$ = $1;}
  | IDENT '[' ']'       {auto e = new ExprList(new IntExpr(1)); $$ = new ArrayVar($1, e);}
  | IDENT '[' ']' SquareExprList {$4->insert(new IntExpr(1)); $$ = new ArrayVar($1, $4);}

FunFP
  :                     {$$ = new FieldList();}
  | BType FunFPVar      {$$ = new FieldList(); $$->emplace_back(unique_ptr<Field>(new Field($1, $2)));}
  | FunFP ',' BType FunFPVar {$1->emplace_back(unique_ptr<Field>(new Field($3, $4))); $$ = $1;}
  ;

FunRP
  :           {$$ = new ExprList();}
  | ExprList  {$$ = $1;}
  ;

Block
  : '{' BlockItems '}'  {$$ = $2;}
  ;

BlockItems
  : Stmt              {$$ = new Block($1);}
  | Decl              {$$ = new Block($1);}
  | BlockItems Stmt   {$1->append($2); $$ = $1;}
  | BlockItems Decl   {$1->append($2); $$ = $1;}
  ;

Stmt
  : Block             {$$ = $1;}
  | ';'               {$$ = new Stmt();}
  | ExprList ';'      {$$ = $1;}
  | IF '(' Cond ')' Stmt {$$ = new If($3, $5);}
  | IF '(' Cond ')' Stmt ELSE Stmt            {$$ = new IfElse($3, $5, $7);}
  | WHILE '(' Cond ')' Stmt                   {$$ = new While($3, $5);}
  | FOR '(' Expr ';' Cond ';' Expr ')' Stmt   {$$ = new For($3, $5, $7, $9);}
  | CONTINUE ';'      {$$ = new Continue();}
  | BREAK ';'         {$$ = new Break();}
  | RETURN E ';'      {$$ = new Return($2);}
  ;

%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<CompUnit> &ast, const char *s) {
  cerr << "error: " << s << " at line " << yylineno << endl;
}
%option noyywrap
%option nounput
%option noinput
%option yylineno

%{

#include <cstdlib>
#include <string>

// 因为 Flex 会用到 Bison 中关于  的定义
// 所以需要 include Bison 生成的头文件
#include "sysy.tab.hpp"

using namespace std;

%}

/* 空白符和注释 */
WhiteSpace    [ \t\n\r]*
LineComment   "//".*

/* 标识符 */
Identifier    [a-zA-Z_][a-zA-Z0-9_]*
AString       \"(\\.|[^\\"])*\"

/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+

/*float变量*/
Float         [0-9]*\.[0-9]+
%%

{WhiteSpace}    { /* 忽略, 不做任何操作 */ }
{LineComment}   { /* 忽略, 不做任何操作 */ }

"int"           { return INT; }
"float"           { return FLOAT; }
"return"        { return RETURN; }
"if"            {return IF;}
"else"          {return ELSE;}
"while"         {return WHILE;}
"for"           {return FOR;}
"break"         {return BREAK;}
"continue"      {return CONTINUE;}
"const"         {return CONST;}
"void"          {return VOID;}
"=="            {return EQ;}
"!="            {return NEQ;}
"<="            {return LE;}
">="            {return GE;}
"&&"            {return AND;}
"||"            {return OR;}
"+="            {return PASSIGN;}
"-="            {return MASSIGN;}
"*="            {return TASSIGN;}
"/="            {return DASSIGN;}


{Identifier}    { yylval.str_val = new string(yytext); return IDENT; }
{AString}       { yylval.str_val = new string(yytext); return STRING; }
{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Float}         { yylval.float_val = atof(yytext); return FLOAT_CONST;}
.               { return yytext[0]; }

%%

/* "="             return ASSIGN;
"<"             return LT;
">"             return GT;
"("             return LL;
")"             return RR;
"["             return LSQUARE;
"]"             return RSQUARE;
"{"             return LBRACE;
"}"             return RBRACE;
"."             return DOT;
","             return COMMA;
";"             return SEMI;
"++"            return PLUSPLUS;
"--"            return MINUSMINUS;
"+"             return PLUS;
"-"             return MINUS;
"*"             return MUL;
"/"             return DIV;
"%"             return MOD;
"!"             return NOT; */

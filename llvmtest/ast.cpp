#pragma once
// #include <cassert>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include "test.cpp"
using namespace std;
using namespace llvm;

class BaseAST
{
public:
  virtual ~BaseAST() = default;
  virtual void Dump() const = 0;
  virtual Value *codegen() = 0;

};

// CompUnit BaseAST
class CompUnitAST : public BaseAST
{
public:
  std::unique_ptr<BaseAST> func_def; 

  void Dump() const override
  {
    std::cout << "CompUnitAST { " << std::endl;
    func_def->Dump();
    std::cout << " }"<< std::endl;
  }

  
};

class FuncDefAST : public BaseAST
{
public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block;

  void Dump() const override
  {
    std::cout << "FuncDefAST { "<< std::endl;
    func_type->Dump();
    std::cout << ident<< std::endl;
    block->Dump();
    std::cout << " }"<< std::endl;
  }
};

class FuncTypeAST : public BaseAST
{
public:
  //   std::unique_ptr<BaseAST> func_type;
  //   std::string ident;
  //   std::unique_ptr<BaseAST> block;

  void Dump() const override
  {
    std::cout << "FuncTypeAST { "<< std::endl;

    std::cout << " }"<< std::endl;
  }
};

class BlockAST : public BaseAST
{
public:
  std::unique_ptr<BaseAST> stmt;
  void Dump() const override
  {
    std::cout << "BlockAST { "<< std::endl;
    stmt->Dump();
    std::cout << " }"<< std::endl;
  }
};

class StmtAST : public BaseAST
{
public:
  std::unique_ptr<BaseAST> number;

  void Dump() const override
  {
    std::cout << "StmtAST { "<< std::endl;
    number->Dump();
    std::cout << " }"<< std::endl;
  }
};




class NumberAST : public BaseAST
{
public:
  int  int_const;
  void Dump() const override
  {
    std::cout << "NumberAST {" << std::endl<< int_const<< std::endl << "}";

  } 
//   void *codegen() override
//   {
//     return ConstantInt::get(TheContext,APInt(int_const))
//   }
};

// ...
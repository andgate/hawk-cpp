#pragma once

#include "hkc/ast/expr.h"
#include "hkc/ast/unparsed_expr.h"
#include "hkc/ast/module.h"

#include <vector>
#include <memory>

namespace hkc
{
	// The abstract syntax tree that holds all the modules
	// expressions, everything. Is a POD.
	struct Ast
	{
		ast::Module_Vector modules = ast::Module_Vector();
	};

	typedef std::unique_ptr<Ast> Ast_ptr;
}

/*

/// NumberExprAST - Expression class for numeric literals like "1.0".
class UnparsedExprAST : public ExprAST
{
public:
	UnparsedExprAST(token_vector Val)
		: Val(std::move(Val))
	{}

private:
	token_vector Val;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
  double Val;

public:
  NumberExprAST(double Val) : Val(Val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
    : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee), Args(std::move(Args)) {}
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> Args)
    : Name(name), Args(std::move(Args)) {}
};


/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class TypeSigAST {
  std::shared_ptr<Token> m_name;
  token_vector m_sig;

public:
  TypeSigAST(std::shared_ptr<Token> name, token_vector type_sig)
    : m_name(name), m_sig(std::move(type_sig)) {}
};

typedef std::vector<std::shared_ptr<TypeSigAST>> typesig_vector;

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
    : Proto(std::move(Proto)), Body(std::move(Body)) {}
};
*/
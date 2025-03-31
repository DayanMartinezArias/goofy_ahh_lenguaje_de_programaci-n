#ifndef AST_H
#define AST_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "variable.h"
#include "symbol.h"


class ASTNode {
 public:
  virtual ~ASTNode() = default;
};

class statement : public ASTNode {};

class expression : public ASTNode {
 public:
  virtual Var value() = 0;
};

class print : public statement {
 public:
  print(expression* exp) : exp_(exp) {}
  expression* exp_;
};

class assignment : public statement {
 public:
  assignment(const std::string& var_name, expression* exp) : var_name_(var_name), exp_(exp) {} 
  std::string var_name_;
  expression* exp_;
};

class numbers : public expression {
 public:
  explicit numbers(int val) : val_(val) {}
  Var value() override {return Var(Val_Type::INT, val_);}
  int val_;
};

class text : public expression {
 public:
  explicit text(std::string val) : val_(val.substr(1, val.size() - 2)) {}
  Var value() override {return Var(Val_Type::STRING, val_);}
  std::string val_;
};

class binaryexpr : public expression {
public:
  binaryexpr(expression* left, expression* right, std::string operand) 
    : left_(left), right_(right), operand_(operand) {}

  Var value() override {
    Var left_val = left_->value();
    Var right_val = right_->value();

    if (operand_ == "+") return left_val + right_val;
    else if (operand_ == "-") return Var(Val_Type::INT, left_val.get_int() - right_val.get_int());
    else if (operand_ == "*") return Var(Val_Type::INT, left_val.get_int() * right_val.get_int());
    else if (operand_ == "/") return Var(Val_Type::INT, left_val.get_int() / right_val.get_int());
    else throw std::runtime_error("Unknown operator: " + operand_);
  }
  expression* left_;
  expression* right_;
  std::string operand_;
};

class id_name : public expression {
public:
  explicit id_name(const std::string& name) : name_(name) {}
  Var value() override {
    return Var();
  }   
  std::string name_;
};

class root : public ASTNode {
 public:
  std::vector<ASTNode*> nodes;
  void add_node(ASTNode* node) {nodes.push_back(node); }
};

#endif

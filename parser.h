#ifndef PARSER_H
#define PARSER_H

#include <stdexcept>
#include "lexer.h"
#include "AST.h"

class Parser {
 public:
  explicit Parser(const std::vector<token>& tokens) : tokens_(tokens) {}
  void parse(root& root_node);
 private:
  ASTNode* ParseStatement();
  expression* ParseExpression();
  bool Check(const Type& type);
  void next_error(const Type& type, const std::string& message);
  token Previous();

  std::vector<token> tokens_;
  size_t position_ = 0;
};

token Parser::Previous() {
  return tokens_[position_  -1];
}

void Parser::next_error(const Type& type, const std::string& message) {
  if (!Check(type)) throw std::invalid_argument(message);
}

bool Parser::Check(const Type& type) {
  if (position_ < tokens_.size() && tokens_[position_].token_type == type) {
    position_++;
    return true;
  }
  else return false;
}

void Parser::parse(root& root_node) {
  while (position_ < tokens_.size()) {
    ASTNode* statement = ParseStatement();
    root_node.add_node(statement);
  }
}

ASTNode* Parser::ParseStatement() {
  if (Check(Type::PRINT)) {
   expression* exp = ParseExpression();
    next_error(Type::END, "Missing semicolon after statement");
    return new print(exp);
  } else if (Check(Type::ID)) {
    std::string name = Previous().value;
    next_error(Type::ASSIGN, "Missing assing operator in expression");
    expression* exp = ParseExpression();
    next_error(Type::END, "Missing semicolon after statement");
    return new assignment(name, exp);
  }
}

expression* Parser::ParseExpression() {
  if (Check(Type::NUMBER)) {
    int val = std::stoi(Previous().value);
    if (Check(Type::OP)) {
      std::string operand = Previous().value;
      expression* right = ParseExpression();
      return new binaryexpr(new numbers(val), right, operand);
    }
    return new numbers(val);
  } 
  else if (Check(Type::STRING)) {
    std::string val = Previous().value;
    if (Check(Type::OP)) {
      std::string operand = Previous().value;
      if (operand != "+") {
        throw std::runtime_error("Strings only support + operator");
      }
      expression* right = ParseExpression();
      return new binaryexpr(new text(val), right, operand);
    }
    return new text(val);
  } else if (Check(Type::ID)) {
    std::string var_name = Previous().value;
    return new id_name(var_name);
  }
  throw std::runtime_error("Expected an expression");
}

#endif
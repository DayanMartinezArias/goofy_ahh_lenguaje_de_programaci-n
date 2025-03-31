// interpreter.h (updated)
#pragma once 

#include "symbol.h"
#include "parser.h"
#include <iostream>

// Esto seguro tiene fugas de memoria, pero me da pereza

class Interpreter {
 public:
  Interpreter(const Symbol& table) : table_(table) {}
    
  void run(const root& node) {
    for (ASTNode* stmt : node.nodes) {
      if (auto* pr = dynamic_cast<print*>(stmt)) {
        if (auto* id = dynamic_cast<id_name*>(pr->exp_)) {
          std::cout << table_.GetVariable(id->name_) << std::endl;
        } else {
          Var result = pr->exp_->value();
          std::cout << result << std::endl;
        }
      } else if (auto* as = dynamic_cast<assignment*>(stmt)) {
        if (auto* id = dynamic_cast<id_name*>(as->exp_)) {
          table_.SetVariable(as->var_name_, table_.GetVariable(id->name_));
        } else {
          Var result = as->exp_->value();
          table_.SetVariable(as->var_name_, result);
        }
      } 
    }
  } 
 private:
  Symbol table_;
};
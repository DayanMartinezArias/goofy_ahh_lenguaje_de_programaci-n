#ifndef SYMBOL_H
#define SYMBOL_H

#include <unordered_map>
#include <stdexcept>
#include <string>
#include <iostream>

#include "variable.h"

class Symbol {
 public:
  void SetVariable(const std::string& name, const Var& value) {
    variables[name] = value;
  }
  Var GetVariable(const std::string& name) const {
    if (variables.find(name) == variables.end()) {
      throw std::runtime_error("Undefined variable: " + name);
    }
    return variables.at(name);
  }
 private:
  std::unordered_map<std::string, Var> variables; 
};

#endif
#include "lexer.h"
#include "AST.h"
#include "parser.h"
#include "symbol.h"
#include "interpreter.h"

#include <fstream>

int main(int argc, char* argv[]) {
  std::string file_name = argv[1];
  std::ifstream file(file_name);
    
  std::stringstream stream;
  stream << file.rdbuf();
  std::string content = stream.str();

  std::vector<token> tokens = tokenize(content);
 
  Parser parser(tokens);
  root root_node;
  parser.parse(root_node);

  Symbol symbol;
  Interpreter interpreter(symbol);
  interpreter.run(root_node);

  return 0;
}

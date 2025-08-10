#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_import ()
{
  AST import (ASTType::AST_IMPORT);

  _pos++;
  VERIFY_POS (_pos);
  auto module = parse_module ();
  import.append (module);

  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_SEMI);
  _pos++;

  return import;
}

}

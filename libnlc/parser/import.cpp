#include "parser.hpp"

namespace nlc
{

CST
Parser::parse_import ()
{
  CST import (CSTType::CST_IMPORT);

  _pos++;
  __VERIFY_POS (_pos);
  auto module = parse_module ();
  import.append (module);
  __VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  __VERIFY_TOKENTYPE (_pos, cur.type, TokenType::TOKEN_SEMI);
  _pos++;

  return import;
}

}

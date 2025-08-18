#include "libnlc/parser/macros.hpp"
#include "parser/parser.hpp"

namespace nlc
{

AST
Parser::parse_module ()
{
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);

  AST module (_pos++, ASTType::AST_MODULE, cur.value);

  auto next = peek (_pos);
  if (next == TokenType::TOKEN_DCOLON)
    {
      _pos++;
      VERIFY_POS (_pos);
      auto submodule = parse_module ();
      module.append (submodule);
    }

  return module;
}

}

#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_type_raw ()
{
  auto cur = _tokens.at (_pos);
  if (cur.type == TokenType::TOKEN_MUL)
    {
      AST type_raw_pointer (_pos++, ASTType::AST_TYPE_POINTER);
      VERIFY_POS (_pos);
      auto type_raw = parse_type_raw ();
      type_raw_pointer.append (type_raw);
      return type_raw_pointer;
    }

  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);

  return AST (_pos++, ASTType::AST_TYPE_PLAIN, cur.value);
}

}

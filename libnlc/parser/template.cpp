#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_template ()
{
  AST template_ (ASTType::AST_TEMPLATE);

  _pos++;

  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LTHAN);
  _pos++;

  AST type_list (ASTType::AST_TEMPLATE_TYPE_LIST);
  while (_pos < _tokens.size ())
    {
      cur = _tokens.at (_pos);
      VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);
      type_list.append (AST (ASTType::AST_TEMPLATE_TYPE, cur.value));

      _pos++;
      auto next = peek (_pos);
      if (next == TokenType::TOKEN_GTHAN)
        {
          break;
        }

      VERIFY_POS (_pos);
      VERIFY_TOKEN (_pos, next, TokenType::TOKEN_COMMA);
      _pos++;
    }
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_GTHAN);
  _pos++;

  template_.append (type_list);

  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);
  if (cur.value == "struct")
    {
      // TODO: implement structs
      return template_;
    }

  VERIFY_POS (_pos + 1);
  cur = _tokens.at (_pos + 1);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_DCOLON);
  auto func_decldef = parse_function_decldef ();
  template_.append (func_decldef);

  return template_;
}

}

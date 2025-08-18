#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_argument_list ()
{
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LPAREN);
  AST arglist (_pos, ASTType::AST_FUNC_ARGLIST);

  _pos++;
  VERIFY_POS (_pos);
  while (_pos < _tokens.size ())
    {
      cur = _tokens.at (_pos);
      if (cur.type == TokenType::TOKEN_RPAREN)
        {
          break;
        }

      AST argument = parse_function_argument ();
      arglist.append (argument);
      auto next = peek (_pos);
      if (next != TokenType::TOKEN_RPAREN)
        {
          VERIFY_POS (_pos);
          VERIFY_TOKEN (_pos, next, TokenType::TOKEN_COMMA);
          _pos++;
          continue;
        }
    }

  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RPAREN);
  _pos++;

  return arglist;
}

}

#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_function_prototype ()
{
  AST funcproto (_pos, ASTType::AST_FUNC_PROTO, _tokens.at (_pos).value);

  _pos += 2;
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LPAREN);

  _pos++;
  VERIFY_POS (_pos);
  AST arguments (_pos, ASTType::AST_FUNC_ARGLIST);
  while (_pos < _tokens.size ())
    {
      cur = _tokens.at (_pos);
      if (cur.type == TokenType::TOKEN_RPAREN)
        {
          break;
        }

      AST argument = parse_function_argument ();
      arguments.append (argument);
      auto next = peek (_pos);
      if (next != TokenType::TOKEN_RPAREN)
        {
          VERIFY_TOKEN (_pos, next, TokenType::TOKEN_COMMA);
          _pos++;
          continue;
        }
    }

  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RPAREN);
  funcproto.append (arguments);

  _pos++;
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RARROW);

  _pos++;
  VERIFY_POS (_pos);
  auto return_type = parse_type ();

  funcproto.append (return_type);

  return funcproto;
}

}

#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_call ()
{
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);
  AST call (ASTType::AST_EXPR_OPERAND_CALL, cur.value);

  _pos++;
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LPAREN);

  // Arguments
  _pos++;
  VERIFY_POS (_pos);
  bool has_prev = false;
  while (_pos < _tokens.size ())
    {
      cur = _tokens.at (_pos);
      if (cur.type == TokenType::TOKEN_RPAREN)
        {
          break;
        }

      // Expect comma before expression
      if (has_prev)
        {
          VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_COMMA);
          _pos++;
          VERIFY_POS (_pos);
        }

      auto expression = parse_expression ();
      call.append (expression);
      has_prev = true;
    }

  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RPAREN);
  _pos++;

  return call;
}

}

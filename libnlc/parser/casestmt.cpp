#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_case_statement ()
{
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);

  AST out;

  if (cur.value == "default")
    {
      _pos++;

      VERIFY_POS (_pos);
      cur = _tokens.at (_pos);
      VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_COLON);
      _pos++;
      out = AST (ASTType::AST_STMT_DEFAULT);
    }
  else if (cur.value == "case")
    {
      out = AST (ASTType::AST_STMT_CASE);
      _pos++;

      VERIFY_POS (_pos);
      auto expr = parse_expression ();
      out.append (expr);

      VERIFY_POS (_pos);
      cur = _tokens.at (_pos);
      VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_COLON);
      _pos++;
    }
  else
    {
      add_error (ParserError::ErrType::PARSER_ERROR_UNEXPECTED, _pos);
      return {};
    }

  auto statement = parse_statement ();
  out.append (statement);
  return out;
}

}

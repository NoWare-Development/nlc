#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_goto_statement ()
{
  AST goto_statement (ASTType::AST_STMT_GOTO);
  _pos++;
  VERIFY_POS (_pos);
  auto label = parse_label ();
  goto_statement.append (label);
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_SEMI);
  _pos++;
  return goto_statement;
}

}

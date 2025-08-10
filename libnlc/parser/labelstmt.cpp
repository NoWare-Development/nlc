#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_label_statement ()
{
  AST label_statement (ASTType::AST_STMT_LABEL);
  VERIFY_POS (_pos);
  auto label = parse_label ();
  label_statement.append (label);
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_COLON);
  _pos++;
  return label_statement;
}

}

#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_initialization_list ()
{
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LBRACE);
  AST initlist (_pos, ASTType::AST_INITLIST);
  _pos++;

  while (_pos < _tokens.size ())
    {
      cur = _tokens.at (_pos);
      if (cur.type == TokenType::TOKEN_RBRACE)
        {
          break;
        }

      auto entry = parse_initialization_list_entry ();
      initlist.append (entry);
      auto next = peek (_pos);
      if (next != TokenType::TOKEN_RBRACE)
        {
          VERIFY_POS (_pos);
          VERIFY_TOKEN (_pos, next, TokenType::TOKEN_COMMA);
          _pos++;
        }
    }

  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RBRACE);

  _pos++;

  return initlist;
}

AST
Parser::parse_initialization_list_entry ()
{
  VERIFY_POS (_pos);

  auto cur = _tokens.at (_pos);
  if (cur.type == TokenType::TOKEN_PERIOD)
    {
      _pos++;

      VERIFY_POS (_pos);
      cur = _tokens.at (_pos);
      VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);
      AST explicit_init (_pos, ASTType::AST_INITLIST_ENTRY_INIT_EXPLICIT,
                         cur.value);
      _pos++;

      VERIFY_POS (_pos);
      cur = _tokens.at (_pos);
      VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_EQ);
      _pos++;

      auto expr = parse_expression ();
      explicit_init.append (expr);

      return explicit_init;
    }

  AST init (_pos, ASTType::AST_INITLIST_ENTRY_INIT);
  auto expr = parse_expression ();
  init.append (expr);

  return init;
}

}

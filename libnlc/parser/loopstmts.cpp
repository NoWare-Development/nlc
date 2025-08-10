#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_break_statement ()
{
  auto next = peek (_pos + 1);
  VERIFY_TOKEN (_pos + 1, next, TokenType::TOKEN_SEMI);
  _pos += 2;
  return AST (ASTType::AST_STMT_BREAK);
}

AST
Parser::parse_continue_statement ()
{
  auto next = peek (_pos + 1);
  VERIFY_TOKEN (_pos + 1, next, TokenType::TOKEN_SEMI);
  _pos += 2;
  return AST (ASTType::AST_STMT_CONTINUE);
}

AST
Parser::parse_while_statement ()
{
  AST while_statement (ASTType::AST_STMT_WHILE);

  _pos++;
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LPAREN);
  _pos++;
  VERIFY_POS (_pos);

  auto expr = parse_expression ();
  while_statement.append (expr);
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RPAREN);

  _pos++;
  VERIFY_POS (_pos);
  auto statement = parse_statement ();
  while_statement.append (statement);
  return while_statement;
}

AST
Parser::parse_for_statement ()
{
  AST for_statement (ASTType::AST_STMT_FOR);
  _pos++;
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LPAREN);
  _pos++;
  auto statement = parse_statement ();
  for_statement.append (statement);
  VERIFY_POS (_pos);

  cur = _tokens.at (_pos);
  if (cur.type == TokenType::TOKEN_SEMI)
    {
      for_statement.append (AST (ASTType::AST_EXPR_OPERAND_NUM, "1"));
    }
  else
    {
      auto cond_expr = parse_expression ();
      for_statement.append (cond_expr);
    }
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_SEMI);
  _pos++;
  VERIFY_POS (_pos);

  cur = _tokens.at (_pos);
  if (cur.type != TokenType::TOKEN_RPAREN)
    {
      auto inc_expr = parse_expression (true);
      for_statement.append (inc_expr);
    }
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RPAREN);
  _pos++;
  VERIFY_POS (_pos);
  auto body = parse_statement ();
  for_statement.append (body);
  return for_statement;
}

AST
Parser::parse_do_while_statement ()
{
  AST do_while_statement (ASTType::AST_STMT_DOWHILE);

  _pos++;
  VERIFY_POS (_pos);

  auto statement = parse_statement ();
  VERIFY_POS (_pos);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_ID);
  VERIFY_VALUE (_pos, cur.value, "while");
  _pos++;
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LPAREN);
  _pos++;
  VERIFY_POS (_pos);
  auto expr = parse_expression ();
  do_while_statement.append (expr);
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RPAREN);
  _pos++;
  VERIFY_POS (_pos);
  cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_SEMI);
  _pos++;

  do_while_statement.append (statement);

  return do_while_statement;
}

}

#include "parser.hpp"

namespace nlc
{

/* ### PUBLIC ### */

CSTNode
Parser::parse (const std::vector<Token> &tokens)
{
  toks = tokens;
  tokenslen = toks.size ();
  pos = 0;

  CSTNode root (CSTNodeType::CST_NODE_TYPE_PROGRAM);
  while (pos < tokenslen)
    {
      root.append (parse_statement ());
    }

  return root;
}

/* ### PRIVATE ### */

CSTNode
Parser::parse_statement_list (TokenType terminator)
{
  CSTNode statement_list (CSTNodeType::CST_NODE_TYPE_STMTLIST);
  while (pos < tokenslen)
    {
      auto cur = toks.at (pos);
      if (cur.type == TokenType::TOKEN_TYPE_RBRACE)
        break;

      statement_list.append (parse_statement ());
    }
  return statement_list;
}

CSTNode
Parser::parse_statement ()
{
  auto cur = toks.at (pos);
  switch (cur.type)
    {
    case TokenType::TOKEN_TYPE_ASTERISK:
      // TODO: implementation -- dereference
      return {};

    case TokenType::TOKEN_TYPE_LBRACE:
      {
        pos++;
        auto stmtlist = parse_statement_list (TokenType::TOKEN_TYPE_LBRACE);
        pos++;
        return stmtlist;
      }

    case TokenType::TOKEN_TYPE_SEMICOL:
      // TODO: warning -- dangling semicolon
      return {};

    case TokenType::TOKEN_TYPE_ID:
      {
        if (cur.value == "if")
          {
            return parse_if_statement ();
          }
        else if (cur.value == "else")
          {
            return parse_else_statement ();
          }
        else if (cur.value == "switch")
          {
            // TODO: implementation -- switch statement
            return {};
          }
        else if (cur.value == "case")
          {
            // TODO: implementation -- case statement
            return {};
          }
        else if (cur.value == "return")
          {
            return parse_return_statement ();
          }
        return parse_identifier_statement ();
      }

    default:
      // TODO: error -- unexpected token
      return {};
    }
}

CSTNode
Parser::parse_else_statement ()
{
  CSTNode else_statement (CSTNodeType::CST_NODE_TYPE_STMT_ELSE);
  pos++;
  else_statement.append (parse_statement ());
  return else_statement;
}

CSTNode
Parser::parse_return_statement ()
{
  pos++;
  skip_until (TokenType::TOKEN_TYPE_SEMICOL); // TODO: parse expression
  pos++;
  return CSTNode (CSTNodeType::CST_NODE_TYPE_STMT_RETURN);
}

CSTNode
Parser::parse_identifier_statement ()
{
  auto next1 = toks.at (pos + 1);

  switch (next1.type)
    {
    case TokenType::TOKEN_TYPE_ID:
      // TODO: implementation -- identifier creation statement
      return {};

    case TokenType::TOKEN_TYPE_LBRACK:
      // TODO: implementation -- array statement
      return {};

    case TokenType::TOKEN_TYPE_LPAREN:
      // TODO: implementation -- call
      return {};

    case TokenType::TOKEN_TYPE_COLON:
      return parse_label_statement ();

    default:
      break;
    }

  // TODO: implementation -- identifier assignment
  return {};
}

CSTNode
Parser::parse_if_statement ()
{
  if (toks.at (pos + 1).type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }

  pos += 2;
  skip_until (TokenType::TOKEN_TYPE_RPAREN); // TODO: parse expression
  pos++;

  CSTNode if_statement (CSTNodeType::CST_NODE_TYPE_STMT_IF);
  auto statement = parse_statement ();
  if_statement.append (statement);
  return if_statement;
}

CSTNode
Parser::parse_label_statement ()
{
  auto name = toks.at (pos).value;
  pos += 2;
  return CSTNode (CSTNodeType::CST_NODE_TYPE_IDSTMT_LABEL, name);
}

// TODO: remove
void
Parser::skip_until (TokenType toktype)
{
  while (pos < tokenslen)
    {
      auto cur = toks.at (pos);
      if (cur.type == toktype)
        break;
      pos++;
    }
}

/* ### COMPARISONS ### */

bool
Parser::is_operator (TokenType toktype) const
{
  return operators.find (toktype) != operators.end ();
}

bool
Parser::is_assign_operator (TokenType toktype) const
{
  return assign_operators.find (toktype) != assign_operators.end ();
}

bool
Parser::is_compare_operator (TokenType toktype) const
{
  return compare_operators.find (toktype) != compare_operators.end ();
}

bool
Parser::is_boolean_operator (TokenType toktype) const
{
  return boolean_operators.find (toktype) != boolean_operators.end ();
}

bool
Parser::is_keyword (const std::string &str) const
{
  for (auto &kw : keywords)
    {
      if (str == kw)
        return true;
    }
  return false;
}

}

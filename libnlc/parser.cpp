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

  CSTNode root{};

  while (pos < tokenslen)
    {
      auto node = parse_statement ();
      root.append (node);
    }

  return root;
}

/* ### PRIVATE ### */

CSTNode
Parser::parse_statement ()
{
  auto cur = toks.at (pos);
  switch (cur.type)
    {
    case TokenType::TOKEN_TYPE_ASTERISK:
      // TODO: implementation -- dereference
      return {};

    case TokenType::TOKEN_TYPE_SEMICOL:
      // TODO: warning -- dangling semicolon
      return {};

    case TokenType::TOKEN_TYPE_ID:
      if (cur.value == "if")
        {
          // TODO: implementation -- if statement
          return {};
        }
      else if (cur.value == "else")
        {
          // TODO: implementation -- else and else if statements
          return {};
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
          // TODO: implementation -- return statement
          return {};
        }
      return parse_identifier_statement ();

    default:
      // TODO: error -- unexpected token
      return {};
    }
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
Parser::parse_label_statement ()
{
  auto name = toks.at (pos).value;
  pos += 2;
  return CSTNode (CSTNodeType::CST_NODE_TYPE_IDSTMT_LABEL, name);
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

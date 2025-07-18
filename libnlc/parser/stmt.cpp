#include "parser.hpp"

namespace nlc
{

CST
Parser::parse_statement ()
{
  auto cur = _tokens.at (_pos);
  switch (cur.type)
    {
    case TokenType::TOKEN_ID:
      {
        if (cur.value == "return")
          {
            return parse_return_statement ();
          }
        else if (cur.value == "if")
          {
            return parse_if_statement ();
          }
        else if (cur.value == "else")
          {
            return parse_else_statement ();
          }
        else if (cur.value == "goto")
          {
            return parse_goto_statement ();
          }
        else if (cur.value == "break")
          {
            return parse_break_statement ();
          }
        else if (cur.value == "continue")
          {
            return parse_continue_statement ();
          }
        else if (cur.value == "for")
          {
            return parse_for_statement ();
          }
        else if (cur.value == "while")
          {
            return parse_while_statement ();
          }
        else if (cur.value == "import")
          {
            return parse_import_statement ();
          }
        else if (cur.value == "break")
          {
            return parse_break_statement ();
          }
        else if (cur.value == "continue")
          {
            return parse_continue_statement ();
          }
      }
      break;

    case TokenType::TOKEN_AT:
      {
        return parse_label_statement ();
      }
      break;

    default:
      break;
    }
  add_error (Parser::ParserError (
      _pos, Parser::ParserErrorType::PARSER_ERROR_UNEXPECTED, cur.value));
  _pos++;
  return {};
}

CST
Parser::parse_return_statement ()
{
  CST return_statement (CSTType::CST_STMT_RETURN);
  skip_until (TokenType::TOKEN_SEMI); // TODO: parse expression
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_SEMI))
    {
      return {};
    }
  _pos++;
  return return_statement;
}

CST
Parser::parse_if_statement ()
{
  CST if_statement (CSTType::CST_STMT_IF);
  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_LPAREN))
    {
      return {};
    }

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  skip_until (TokenType::TOKEN_RPAREN); // TODO: parse expression
  if (!verify_pos (_pos))
    {
      return {};
    }
  _pos++;

  if (!verify_pos (_pos))
    {
      return {};
    }
  auto statement = parse_statement ();
  if_statement.append (statement);

  return if_statement;
}

CST
Parser::parse_else_statement ()
{
  CST else_statement (CSTType::CST_STMT_ELSE);

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto statement = parse_statement ();
  else_statement.append (statement);

  return else_statement;
}

CST
Parser::parse_goto_statement ()
{
  CST goto_statement (CSTType::CST_STMT_GOTO);
  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_AT))
    {
      return {};
    }

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_ID))
    {
      return {};
    }
  goto_statement.value = cur.value;

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_SEMI))
    {
      return {};
    }

  _pos++;

  return goto_statement;
}

CST
Parser::parse_break_statement ()
{
  CST break_statement (CSTType::CST_STMT_BREAK);

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_SEMI))
    {
      return {};
    }
  _pos++;

  return break_statement;
}

CST
Parser::parse_continue_statement ()
{
  CST continue_statement (CSTType::CST_STMT_CONTINUE);

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_SEMI))
    {
      return {};
    }
  _pos++;

  return continue_statement;
}

CST
Parser::parse_for_statement ()
{
  CST for_statement (CSTType::CST_STMT_FOR);

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_LPAREN))
    {
      return {};
    }

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }

  skip_until (TokenType::TOKEN_RPAREN); // TODO: parse expression
  cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_RPAREN))
    {
      return {};
    }

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }

  auto statement = parse_statement ();
  for_statement.append (statement);

  return for_statement;
}

CST
Parser::parse_while_statement ()
{
  CST while_statement (CSTType::CST_STMT_WHILE);

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_LPAREN))
    {
      return {};
    }
  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  skip_until (TokenType::TOKEN_RPAREN); // TODO: parse expression
  if (!verify_pos (_pos))
    {
      return {};
    }
  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto statement = parse_statement ();
  while_statement.append (statement);

  return while_statement;
}

CST
Parser::parse_label_statement ()
{
  CST label_statement (CSTType::CST_STMT_LABEL);
  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_ID))
    {
      return {};
    }
  label_statement.value = cur.value;
  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }
  cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_COLON))
    {
      return {};
    }
  _pos++;
  return label_statement;
}

CST
Parser::parse_import_statement ()
{
  CST import_statement (CSTType::CST_STMT_IMPORT);

  _pos++;
  if (!verify_pos (_pos))
    {
      return {};
    }

  auto module = parse_module ();
  import_statement.append (module);

  if (!verify_pos (_pos))
    {
      return {};
    }
  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_SEMI))
    {
      return {};
    }
  _pos++;

  return import_statement;
}

}

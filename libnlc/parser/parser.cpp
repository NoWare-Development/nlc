#include "parser.hpp"

namespace nlc
{

CST
Parser::parse ()
{
  CST prog (CSTType::CST_PROG);
  while (_pos < _tokens.size ())
    {
      auto statement = parse_statement ();
      if (_errored)
        {
          _errored = false;
          continue;
        }
      prog.append (statement);
    }
  return prog;
}

std::vector<Parser::ParserError>
Parser::get_errors () const
{
  return _errors;
}

void
Parser::skip_until (TokenType type)
{
  while (_pos < _tokens.size ())
    {
      auto curtype = _tokens.at (_pos).type;
      if (curtype == type)
        {
          return;
        }
      _pos++;
    }
}

void
Parser::add_error (const Parser::ParserError &err)
{
  _errors.push_back (err);
  _errored = true;
}

bool
Parser::verify_pos (size_t pos)
{
  if (pos >= _tokens.size ())
    {
      add_error (Parser::ParserError (
          pos, Parser::ParserErrorType::PARSER_ERROR_EXPECTED));
      return false;
    }
  return true;
}

bool
Parser::verify_tokentype (size_t pos, TokenType got, TokenType expected)
{
  if (got != expected)
    {
      add_error (Parser::ParserError (
          pos, Parser::ParserErrorType::PARSER_ERROR_UNEXPECTED));
      return false;
    }
  return true;
}

}

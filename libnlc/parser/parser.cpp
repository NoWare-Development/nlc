#include "parser.hpp"
#include <string>

namespace nlc
{

CST
Parser::parse ()
{
  CST prog (CSTType::CST_PROG);
  while (_pos < _tokens.size ())
    {
      auto result = parse_statement ();
      if (_errored)
        {
          _errored = false;
          continue;
        }
      prog.append (result);
    }
  return prog;
}

std::vector<Parser::ParserError>
Parser::get_errors () const
{
  return _errors;
}

CST
Parser::parse_statement ()
{
  auto cur = _tokens.at (_pos);
  if (cur.type == TokenType::TOKEN_ID)
    {
      if (cur.value == "return")
        {
          return parse_return_statement ();
        }
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
  skip_until (TokenType::TOKEN_SEMI);
  _pos++;
  return return_statement;
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

std::string
Parser::ParserError::to_string () const
{
  std::string out{};
  out += "Parser::ParserError { ";

  out += "msg: \"";
  out += msg;
  out += "\"";

  out += ", pos: ";
  out += std::to_string (pos);

  out += ", type: ";
  out += std::to_string (type);

  out += " }";
  return out;
}

}

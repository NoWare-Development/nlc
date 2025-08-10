#include "lexer.hpp"
#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_identifier_statement ()
{
  VERIFY_POS (_pos + 1);
  auto next = peek (_pos + 1);
  switch (next)
    {
    case TokenType::TOKEN_COLON:
      {
        auto vardecldef = parse_variable_decldef ();
        VERIFY_POS (_pos);
        next = peek (_pos);
        VERIFY_TOKEN (_pos, next, TokenType::TOKEN_SEMI);
        _pos++;
        return vardecldef;
      }

    case TokenType::TOKEN_DCOLON:
      {
        return parse_function_decldef ();
      }

    default:
      break;
    }

  add_error (ParserError::ErrType::PARSER_ERROR_UNEXPECTED, _pos);
  _pos++;
  return {};
}

}

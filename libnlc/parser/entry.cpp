#include "parser.hpp"

namespace nlc
{

CST
Parser::parse_entry ()
{
  auto cur = _tokens.at (_pos);

  // TODO: parse dangling semicolon

  if (cur.type == TokenType::TOKEN_ID)
    {
      if (cur.value == "import")
        {
          return parse_import ();
        }
    }

  add_error (_pos, ParserErrorType::PARSER_ERROR_UNEXPECTED);
  _pos++;
  return {};
}

}

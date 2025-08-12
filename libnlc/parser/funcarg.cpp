#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_function_argument ()
{
  auto cur = _tokens.at (_pos);
  if (cur.type == TokenType::TOKEN_PERIOD)
    {
      TokenType next_two[2] = {
        peek (_pos + 1),
        peek (_pos + 2),
      };

      if (next_two[0] == TokenType::TOKEN_PERIOD && next_two[0] == next_two[1])
        {
          AST out (_pos, ASTType::AST_VARIADIC);
          _pos += 3;
          return out;
        }
    }

  return parse_variable_decldef ();
}

}

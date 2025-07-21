#include "parser.hpp"
#include <iostream>

namespace nlc
{

CST
Parser::parse_module ()
{
  auto cur = _tokens.at (_pos);

  switch (cur.type)
    {
    case TokenType::TOKEN_MUL:
      _pos++;
      return CST (CSTType::CST_MODULE_EVERYTHING);

    case TokenType::TOKEN_ID:
      {
        auto next = peek (_pos + 1);
        if (next == TokenType::TOKEN_DCOLON)
          {
            _pos += 2;
            __VERIFY_POS (_pos);
            CST module (CSTType::CST_MODULE_NAME, cur.value);
            module.append (parse_module ());
            return module;
          }

        _pos++;
        return CST (CSTType::CST_MODULE_ITEM, cur.value);
      }

    default:
      break;
    }

  add_error (_pos, ParserErrorType::PARSER_ERROR_UNEXPECTED);
  _pos++;
  return {};
}

}

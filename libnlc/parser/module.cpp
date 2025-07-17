#include "parser.hpp"

namespace nlc
{

CST
Parser::parse_module ()
{
  CST module (CSTType::CST_MODULE);

  auto cur = _tokens.at (_pos);
  if (!verify_tokentype (_pos, cur.type, TokenType::TOKEN_ID))
    {
      return {};
    }
  module.value = cur.value;
  _pos++;

  if (_pos < _tokens.size ())
    {
      auto next1 = _tokens.at (_pos);
      if (next1.type == TokenType::TOKEN_PERIOD)
        {
          _pos++;
          if (!verify_pos (_pos))
            {
              return {};
            }
          auto submodule = parse_module ();
          module.append (submodule);
        }
    }

  return module;
}

}

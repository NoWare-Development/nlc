#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_export ()
{
  AST export_ (ASTType::AST_EXPORT);

  _pos++;
  VERIFY_POS (_pos);

  auto top = parse_top (true);

  export_.append (top);

  return export_;
}

}

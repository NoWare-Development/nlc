#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_export ()
{
  AST export_ (_pos++, ASTType::AST_EXPORT);

  VERIFY_POS (_pos);

  auto top = parse_top (true);

  export_.append (top);

  return export_;
}

}

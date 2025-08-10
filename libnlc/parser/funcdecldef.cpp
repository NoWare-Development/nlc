#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_function_decldef ()
{
  auto funcproto = parse_function_prototype ();

  auto next = peek (_pos);
  if (next == TokenType::TOKEN_SEMI)
    {
      AST funcdecl (ASTType::AST_FUNC_DECL);
      funcdecl.append (funcproto);
      _pos++;
      return funcdecl;
    }

  AST funcdef (ASTType::AST_FUNC_DEF);
  funcdef.append (funcproto);

  VERIFY_TOKEN (_pos, next, TokenType::TOKEN_LBRACE);

  auto stmtlist = parse_statement_list ();
  funcdef.append (stmtlist);

  return funcdef;
}

}

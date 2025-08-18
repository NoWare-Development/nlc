#include "libnlc/parser/macros.hpp"
#include "parser.hpp"

namespace nlc
{

AST
Parser::parse_statement_list ()
{
  AST stmtlist (_pos, ASTType::AST_STMT_LIST);
  auto cur = _tokens.at (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_LBRACE);
  _pos++;
  VERIFY_POS (_pos);

  while (_pos < _tokens.size ())
    {
      cur = _tokens.at (_pos);
      if (cur.type == TokenType::TOKEN_RBRACE)
        {
          break;
        }

      auto statement = parse_statement ();
      stmtlist.append (statement);
    }

  VERIFY_POS (_pos);
  VERIFY_TOKEN (_pos, cur.type, TokenType::TOKEN_RBRACE);
  _pos++;
  return stmtlist;
}

AST
Parser::parse_statement ()
{
  auto cur = _tokens.at (_pos);
  switch (cur.type)
    {
    case TokenType::TOKEN_SEMI:
      {
        return AST (_pos++, ASTType::AST_NONE);
      }

    case TokenType::TOKEN_LBRACE:
      {
        return parse_statement_list ();
      }

    case TokenType::TOKEN_ID:
      {
        if (cur.value == "struct")
          {
            return parse_struct ();
          }
        else if (cur.value == "union")
          {
            return parse_union ();
          }
        else if (cur.value == "enum")
          {
            return parse_enum_definition ();
          }
        else if (cur.value == "return")
          {
            return parse_return_statement ();
          }
        else if (cur.value == "goto")
          {
            return parse_goto_statement ();
          }
        else if (cur.value == "break")
          {
            return parse_break_statement ();
          }
        else if (cur.value == "continue")
          {
            return parse_continue_statement ();
          }
        else if (cur.value == "while")
          {
            return parse_while_statement ();
          }
        else if (cur.value == "for")
          {
            return parse_for_statement ();
          }
        else if (cur.value == "do")
          {
            return parse_do_while_statement ();
          }
        else if (cur.value == "switch")
          {
            return parse_switch_statement ();
          }
        else if (cur.value == "defer")
          {
            return parse_defer_statement ();
          }
        else if (cur.value == "if")
          {
            return parse_if_statement ();
          }
        else if (cur.value == "template")
          {
            return parse_template ();
          }
        else if (is_modifier (cur.value))
          {
            return parse_decldef ();
          }

        auto next = peek (_pos + 1);
        switch (next)
          {
          case TokenType::TOKEN_DCOLON:
            // Check if statement is function declaration/definition.
            // If not, it is access from other module and should be treated as
            // expression statement.
            next = peek (_pos + 2);
            if (next != TokenType::TOKEN_LPAREN)
              {
                break;
              }
          case TokenType::TOKEN_COLON:
            return parse_decldef ();
          default:
            break;
          }

        break;
      }

    case TokenType::TOKEN_AT:
      {
        return parse_label_statement ();
      }

    default:
      break;
    }

  auto out_expression_statement = parse_expression_statement ();
  if (_errored)
    {
      _pos++;
      return {};
    }

  return out_expression_statement;
}

}

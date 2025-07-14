#include "parser.hpp"
#include "lexer.hpp"
#include <iostream>

namespace nlc
{

/* ### PUBLIC ### */

CSTNode
Parser::parse (const std::vector<Token> &tokens)
{
  toks = tokens;
  tokenslen = toks.size ();
  pos = 0;

  CSTNode root (CSTNodeType::CST_NODE_TYPE_PROGRAM);
  while (pos < tokenslen)
    {
      auto node = parse_statement ();
      if (node.type == CSTNodeType::CST_NODE_TYPE_UNK)
        {
          // TODO: proper error handling
          std::cout << "Something went wrong, pos is " << pos << '\n';
          std::cout << "Errored token is " << toks.at (pos).to_string ()
                    << '\n';
          break;
        }
      root.append (node);
    }

  return root;
}

/* ### PRIVATE ### */

CSTNode
Parser::parse_statement_list ()
{
  CSTNode statement_list (CSTNodeType::CST_NODE_TYPE_STMTLIST);
  while (pos < tokenslen)
    {
      auto cur = toks.at (pos);
      if (cur.type == TokenType::TOKEN_TYPE_RBRACE)
        {
          break;
        }

      statement_list.append (parse_statement ());
    }
  return statement_list;
}

CSTNode
Parser::parse_statement ()
{
  if (pos >= tokenslen)
    {
      std::cout << "ERROR: bad statement, position (" << pos
                << ") is out of bounds\n";
      return {};
    }

  auto cur = toks.at (pos);
  switch (cur.type)
    {
    case TokenType::TOKEN_TYPE_LBRACE:
      {
        pos++;
        auto stmtlist = parse_statement_list ();
        auto cur = toks.at (pos);
        if (cur.type != TokenType::TOKEN_TYPE_RBRACE)
          {
            // TODO: error -- unexpected token
            return {};
          }
        pos++;
        return stmtlist;
      }

    case TokenType::TOKEN_TYPE_AT:
      return parse_label_statement ();

    case TokenType::TOKEN_TYPE_SEMICOL:
      // TODO: warning -- dangling semicolon
      return {};

    case TokenType::TOKEN_TYPE_ASTERISK:
      {
        return parse_identifier_statement ();
      }

    case TokenType::TOKEN_TYPE_ID:
      {
        if (cur.value == "if")
          {
            return parse_if_statement ();
          }
        else if (cur.value == "else")
          {
            return parse_else_statement ();
          }
        else if (cur.value == "switch")
          {
            return parse_switch_statement ();
          }
        else if (cur.value == "return")
          {
            return parse_return_statement ();
          }
        else if (cur.value == "break")
          {
            if (pos + 1 >= tokenslen
                || toks.at (pos + 1).type != TokenType::TOKEN_TYPE_SEMICOL)
              {
                // TODO: error -- unexpected token
                return {};
              }

            pos += 2;
            return CSTNode (CSTNodeType::CST_NODE_TYPE_STMT_BREAK);
          }
        else if (cur.value == "continue")
          {
            if (pos + 1 >= tokenslen
                || toks.at (pos + 1).type != TokenType::TOKEN_TYPE_SEMICOL)
              {
                // TODO: error -- unexpected token
                return {};
              }

            pos += 2;
            return CSTNode (CSTNodeType::CST_NODE_TYPE_STMT_CONTINUE);
          }
        else if (cur.value == "while")
          {
            return parse_while_statement ();
          }
        else if (cur.value == "for")
          {
            return parse_for_statement ();
          }
        else if (cur.value == "goto")
          {
            return parse_goto_statement ();
          }
        return parse_identifier_statement ();
      }

    default:
      // TODO: error -- unexpected token
      return {};
    }
}

CSTNode
Parser::parse_if_statement ()
{
  CSTNode if_statement (CSTNodeType::CST_NODE_TYPE_STMT_IF);

  if (toks.at (pos + 1).type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }

  pos += 2;
  skip_until (TokenType::TOKEN_TYPE_RPAREN); // TODO: parse expression
  pos++;

  if_statement.append (parse_statement ());
  return if_statement;
}

CSTNode
Parser::parse_else_statement ()
{
  CSTNode else_statement (CSTNodeType::CST_NODE_TYPE_STMT_ELSE);
  pos++;
  else_statement.append (parse_statement ());
  return else_statement;
}

CSTNode
Parser::parse_switch_statement ()
{
  if (toks.at (pos + 1).type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }

  pos += 2;
  skip_until (TokenType::TOKEN_TYPE_RPAREN); // TODO: parse expression
  pos++;

  if (toks.at (pos).type != TokenType::TOKEN_TYPE_LBRACE)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  CSTNode switch_statement (CSTNodeType::CST_NODE_TYPE_STMT_SWITCH);
  while (pos < tokenslen)
    {
      auto tok = toks.at (pos);
      if (tok.type == TokenType::TOKEN_TYPE_RBRACE)
        {
          break;
        }

      switch_statement.append (parse_case_statement ());
    }
  pos++;

  return switch_statement;
}

CSTNode
Parser::parse_return_statement ()
{
  CSTNode return_statement (CSTNodeType::CST_NODE_TYPE_STMT_RETURN);
  pos++;
  skip_until (TokenType::TOKEN_TYPE_SEMICOL);

  // TODO: enable when expressions are ready and comment line above.
  // return_statement.append (parse_expression ());

  if (toks.at (pos).type != TokenType::TOKEN_TYPE_SEMICOL)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;
  return return_statement;
}

CSTNode
Parser::parse_while_statement ()
{
  CSTNode while_statement (CSTNodeType::CST_NODE_TYPE_STMT_WHILE);
  pos++;

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  skip_until (TokenType::TOKEN_TYPE_RPAREN); // TODO: parse expression
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_RPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  while_statement.append (parse_statement ());
  return while_statement;
}

CSTNode
Parser::parse_for_statement ()
{
  CSTNode for_statement (CSTNodeType::CST_NODE_TYPE_STMT_FOR);
  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  // TODO: parse <stmt>;<expr>;<expr>
  skip_until (TokenType::TOKEN_TYPE_RPAREN);
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_RPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }

  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  auto statement = parse_statement ();

  for_statement.append (statement);
  return for_statement;
}

CSTNode
Parser::parse_goto_statement ()
{
  CSTNode goto_statement (CSTNodeType::CST_NODE_TYPE_STMT_GOTO);

  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_AT)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_ID)
    {
      // TODO: error -- unexpected token
      return {};
    }
  goto_statement.value = cur.value;

  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_SEMICOL)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  return goto_statement;
}

CSTNode
Parser::parse_label_statement ()
{
  CSTNode label_statement (CSTNodeType::CST_NODE_TYPE_STMT_LABEL);
  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_ID)
    {
      // TODO: error -- unexpected token
      return {};
    }
  label_statement.value = cur.value;

  pos++;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_COLON)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  return label_statement;
}

CSTNode
Parser::parse_identifier_statement ()
{
  if (pos + 1 >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto next = toks.at (pos + 1);
  switch (next.type)
    {
    case TokenType::TOKEN_TYPE_COLON:
      return parse_identifier_create_statement ();

    case TokenType::TOKEN_TYPE_COLON_COLON:
      return parse_function_prototype_statement ();

    case TokenType::TOKEN_TYPE_LPAREN:
      {
        auto call = parse_call ();
        if (toks.at (pos).type != TokenType::TOKEN_TYPE_SEMICOL)
          {
            // TODO: error -- unexpected token
            return {};
          }
        pos++;
        CSTNode call_statement (CSTNodeType::CST_NODE_TYPE_IDSTMT_CALL);
        call_statement.append (call);
        return call_statement;
      }

    default:
      return {};
    }
}

CSTNode
Parser::parse_function_prototype_statement ()
{
  auto func_proto = parse_function_prototype ();
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  auto cur = toks.at (pos);
  if (cur.type == TokenType::TOKEN_TYPE_SEMICOL)
    {
      CSTNode func_decl (CSTNodeType::CST_NODE_TYPE_IDSTMT_FUNC_DECLARATION);
      func_decl.append (func_proto);
      pos++;
      return func_decl;
    }

  if (cur.type == TokenType::TOKEN_TYPE_LBRACE)
    {
      CSTNode func_def (CSTNodeType::CST_NODE_TYPE_IDSTMT_FUNC_DEFINITION);
      func_def.append (func_proto);
      pos++;
      if (pos >= tokenslen)
        {
          // TODO: error -- expected token
          return {};
        }

      auto statement_list = parse_statement_list ();
      func_def.append (statement_list);
      if (pos >= tokenslen)
        {
          // TODO: error -- expected token
          return {};
        }
      cur = toks.at (pos);
      if (cur.type != TokenType::TOKEN_TYPE_RBRACE)
        {
          // TODO: error -- unexpected token
          return {};
        }

      pos++;
      return func_def;
    }

  // TODO: error -- unexpected token
  return {};
}

CSTNode
Parser::parse_identifier_create_statement ()
{
  auto identifier_create = parse_identifier_create ();
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto cur = toks.at (pos);
  if (cur.type == TokenType::TOKEN_TYPE_EQ)
    {
      CSTNode id_create_stmt_with_assign (
          CSTNodeType::CST_NODE_TYPE_IDCREATESTMT_WITH_ASSIGN);
      id_create_stmt_with_assign.append (identifier_create);
      pos++;
      if (pos >= tokenslen)
        {
          // TODO: error -- expected token
          return {};
        }
      skip_until (TokenType::TOKEN_TYPE_SEMICOL); // TODO: parse expression
      if (pos >= tokenslen)
        {
          // TODO: error -- expected token
          return {};
        }
      cur = toks.at (pos);
      if (cur.type != TokenType::TOKEN_TYPE_SEMICOL)
        {
          // TODO: error -- unexpected token
          return {};
        }
      pos++;
      return id_create_stmt_with_assign;
    }

  if (cur.type == TokenType::TOKEN_TYPE_SEMICOL)
    {
      CSTNode id_create_stmt_without_assign (
          CSTNodeType::CST_NODE_TYPE_IDCREATESTMT_WITHOUT_ASSIGN);
      id_create_stmt_without_assign.append (identifier_create);
      pos++;
      return id_create_stmt_without_assign;
    }

  // TODO: error -- unexpected token
  return {};
}

CSTNode
Parser::parse_identifier_create ()
{
  auto cur = toks.at (pos);
  if (is_typemodifier (cur.value))
    {
      CSTNode identifier_create_with_mod (
          CSTNodeType::CST_NODE_TYPE_IDCREATE_WITH_MOD);
      CSTNode typemodifier (CSTNodeType::CST_NODE_TYPE_TYPEMODIFIER,
                            cur.value);
      identifier_create_with_mod.append (parse_identifier_create ());
      return identifier_create_with_mod;
    }

  if (pos + 1 >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto next = toks.at (pos + 1);
  if (next.type != TokenType::TOKEN_TYPE_COLON)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos += 2;

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  auto identifier_type = parse_identifier_type ();

  CSTNode identifier_create (CSTNodeType::CST_NODE_TYPE_IDCREATE_PLAIN,
                             cur.value);
  identifier_create.append (identifier_type);
  return identifier_create;
}

CSTNode
Parser::parse_identifier_create_list ()
{
  CSTNode identifier_create_list (CSTNodeType::CST_NODE_TYPE_IDCREATE_LIST);
  while (pos < tokenslen)
    {
      auto cur = toks.at (pos);
      if (cur.type == TokenType::TOKEN_TYPE_COMMA)
        {
          pos++;
          continue;
        }

      if (cur.type == TokenType::TOKEN_TYPE_ID && is_typemodifier (cur.value)
          && cur.value != "const")
        {
          // TODO: error -- static and inline modifiers are not allowed
          return {};
        }

      if (cur.type != TokenType::TOKEN_TYPE_ID)
        {
          break;
        }

      auto identifier_create = parse_identifier_create ();
      if (identifier_create.type == CSTNodeType::CST_NODE_TYPE_UNK)
        {
          // TODO: error -- failed to parse identifier creation
          return {};
        }

      identifier_create_list.append (identifier_create);
    }
  return identifier_create_list;
}

CSTNode
Parser::parse_identifier_type ()
{
  auto cur = toks.at (pos);
  if (cur.type == TokenType::TOKEN_TYPE_ASTERISK)
    {
      pos++;
      if (pos >= tokenslen)
        {
          // TODO: error -- expected token
          return {};
        }
      CSTNode type_ptr (CSTNodeType::CST_NODE_TYPE_IDTYPE_POINTER);
      type_ptr.append (parse_identifier_type ());
      return type_ptr;
    }

  CSTNode type_raw (CSTNodeType::CST_NODE_TYPE_IDTYPE_PLAIN);
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  type_raw.append (parse_identifier_type_raw ());
  return type_raw;
}

CSTNode
Parser::parse_identifier_type_raw ()
{
  auto cur = toks.at (pos);
  if (cur.type == TokenType::TOKEN_TYPE_LPAREN)
    {
      return parse_identifier_type_raw_funcptr ();
    }
  if (pos + 1 < tokenslen)
    {
      auto next = toks.at (pos + 1);
      if (next.type == TokenType::TOKEN_TYPE_LBRACK)
        {
          return parse_identifier_type_raw_array ();
        }
      else if (next.type == TokenType::TOKEN_TYPE_LPAREN)
        {
          return parse_identifier_type_raw_funcptr ();
        }
    }

  return parse_identifier_type_raw_plain ();
}

CSTNode
Parser::parse_identifier_type_raw_plain ()
{
  auto cur = toks.at (pos);
  pos++;
  return CSTNode (CSTNodeType::CST_NODE_TYPE_IDTYPERAW_PLAIN, cur.value);
}

CSTNode
Parser::parse_identifier_type_raw_array ()
{
  CSTNode type_raw_array (CSTNodeType::CST_NODE_TYPE_IDTYPERAW_ARRAY,
                          toks.at (pos).value);

  pos += 2;
  skip_until (TokenType::TOKEN_TYPE_RBRACK); // TODO: parse expression
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_RBRACK)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  return type_raw_array;
}

CSTNode
Parser::parse_identifier_type_raw_funcptr ()
{
  CSTNode type_raw_funcptr (CSTNodeType::CST_NODE_TYPE_IDTYPERAW_FUNCPTR);
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;
  auto id_create_list = parse_identifier_create_list ();
  type_raw_funcptr.append (id_create_list);
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_RPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_RLONGARROW)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto return_type = parse_identifier_type ();
  type_raw_funcptr.append (return_type);

  return type_raw_funcptr;
}

CSTNode
Parser::parse_function_prototype ()
{
  CSTNode func_proto (CSTNodeType::CST_NODE_TYPE_FUNCTION_PROTOTYPE,
                      toks.at (pos).value);
  if (pos + 1 >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto next = toks.at (pos + 1);
  if (next.type != TokenType::TOKEN_TYPE_COLON_COLON)
    {
      // TODO: error -- unexpected token
      return {};
    }

  if (pos + 2 >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  next = toks.at (pos + 2);
  if (next.type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }

  pos += 3;
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto identifier_create_list = parse_identifier_create_list ();
  func_proto.append (identifier_create_list);

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_RPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_RLONGARROW)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto return_type = parse_identifier_type ();
  func_proto.append (return_type);

  return func_proto;
}

CSTNode
Parser::parse_call ()
{
  CSTNode call (CSTNodeType::CST_NODE_TYPE_IDSTMT_CALL);
  auto cur = toks.at (pos);
  call.value = cur.value;
  if (pos + 1 >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }
  auto next = toks.at (pos + 1);
  if (next.type != TokenType::TOKEN_TYPE_LPAREN)
    {
      // TODO: error -- unexpected token
      return {};
    }

  pos += 2;
  skip_until (TokenType::TOKEN_TYPE_RPAREN); // TODO: parse expression list
  if (pos >= tokenslen)
    {
      // TODO: error -- expected token
      return {};
    }

  pos++;
  return call;
}

CSTNode
Parser::parse_case_statement ()
{
  auto cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_ID)
    {
      // TODO: error -- unexpected token
      return {};
    }

  CSTNodeType type{};
  // TODO: parse expression

  // CSTNode expression{};
  if (cur.value == "case")
    {
      // _case <expr>_: { <stmtlist> }
      type = CSTNodeType::CST_NODE_TYPE_CASESTMT_CASE;
      pos++;
      // expression = parse_expression();
      skip_until (TokenType::TOKEN_TYPE_COLON);
    }
  else if (cur.value == "default")
    {
      // _default_: { <stmtlist> }
      type = CSTNodeType::CST_NODE_TYPE_CASESTMT_DEFAULT;
      pos++;
    }
  else
    {
      // TODO: error -- unexpected token
      return {};
    }

  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_COLON)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  cur = toks.at (pos);
  if (cur.type != TokenType::TOKEN_TYPE_LBRACE)
    {
      // TODO: error -- unexpected token
      return {};
    }
  pos++;

  auto statement_list = parse_statement_list ();
  pos++;

  CSTNode case_statement (type);
  // TODO: parse expression
  // if (type == CSTNodeType::CST_NODE_TYPE_CASESTMT_CASE)
  //   {
  //     case_statement.append(expression);
  //   }
  case_statement.append (statement_list);

  return case_statement;
}

CSTNode
Parser::parse_expression ()
{
  CSTNode expression (CSTNodeType::CST_NODE_TYPE_EXPR);

  TokenType prev_type{};
  while (pos < tokenslen)
    {
      auto cur = toks.at (pos);

      // TODO: add more checks about context to infer that output expression
      // CST is ready to become AST.

      if (cur.type == TokenType::TOKEN_TYPE_LPAREN)
        {
          pos++;
          auto nested_expr = parse_expression ();
          cur = toks.at (pos);
          if (cur.type != TokenType::TOKEN_TYPE_RPAREN)
            {
              // TODO: error -- unexpected token.
              return {};
            }
          pos++;

          prev_type = cur.type;
          expression.append (nested_expr);
          continue;
        }

      if (is_operand (cur.type))
        {
          expression.append (parse_operand ());
          continue;
        }

      if (cur.type == TokenType::TOKEN_TYPE_MINUS)
        {
          if (is_operator (prev_type))
            {
              prev_type = cur.type;
              expression.append (CSTNode (prefix_operators.at (cur.type)));
              pos++;
              continue;
            }

          prev_type = cur.type;
          expression.append (CSTNode (operators.at (cur.type)));
          pos++;
          continue;
        }

      if (is_operator (cur.type))
        {
          prev_type = cur.type;
          expression.append (CSTNode (operators.at (cur.type)));
          pos++;
          continue;
        }

      if (is_prefix_operator (cur.type))
        {
          prev_type = cur.type;
          expression.append (CSTNode (prefix_operators.at (cur.type)));
          pos++;
          continue;
        }

      if (is_compare_operator (cur.type))
        {
          prev_type = cur.type;
          expression.append (CSTNode (compare_operators.at (cur.type)));
          pos++;
          continue;
        }

      // End of expression
      break;
    }
  return expression;
}

CSTNode
Parser::parse_operand ()
{
  auto cur = toks.at (pos);
  switch (cur.type)
    {
    case TokenType::TOKEN_TYPE_NUMBER:
      pos++;
      return CSTNode (CSTNodeType::CST_NODE_TYPE_OPERAND_INTEGER, cur.value);

    case TokenType::TOKEN_TYPE_FLOATING:
      pos++;
      return CSTNode (CSTNodeType::CST_NODE_TYPE_OPERAND_FLOATING, cur.value);

    default:
      return parse_idoperand ();
    }
}

CSTNode
Parser::parse_idoperand ()
{
  // FIXME: I have thoughts that it would probably not go that well with how I
  // do it now. It will probably fail in more complex cases. I think for now it
  // is OK, but should be fixed later.

  CSTNode idoperand (CSTNodeType::CST_NODE_TYPE_OPERAND_ID);

  auto first = toks.at (pos);
  if (is_pre_operator (first.type))
    {
      idoperand.append (CSTNode (pre_operators.at (first.type)));

      if (pos + 1 >= tokenslen)
        {
          // TODO: error -- expected token
          return {};
        }
      auto second = toks.at (pos + 1);
      if (second.type != TokenType::TOKEN_TYPE_ID)
        {
          // TODO: error -- unexpected token
          return {};
        }
      pos += 2;

      idoperand.value = second.value;
      return idoperand;
    }
  else if (first.type == TokenType::TOKEN_TYPE_ID)
    {
      idoperand.value = first.value;
      if (pos + 1 < tokenslen)
        {
          auto second = toks.at (pos + 1);
          if (is_post_operator (second.type))
            {
              // Has post operand
              idoperand.append (CSTNode (post_operators.at (second.type)));
              pos += 2;
              return idoperand;
            }
        }
      pos++;
      return idoperand;
    }

  // TODO: error -- unexpected token
  return {};
}

// TODO: remove
void
Parser::skip_until (TokenType toktype)
{
  while (pos < tokenslen)
    {
      auto cur = toks.at (pos);
      if (cur.type == toktype)
        break;
      pos++;
    }
}

/* ### COMPARISONS ### */

bool
Parser::is_operator (TokenType toktype) const
{
  return operators.find (toktype) != operators.end ();
}

bool
Parser::is_prefix_operator (TokenType toktype) const
{
  return prefix_operators.find (toktype) != prefix_operators.end ();
}

bool
Parser::is_assign_operator (TokenType toktype) const
{
  return assign_operators.find (toktype) != assign_operators.end ();
}

bool
Parser::is_compare_operator (TokenType toktype) const
{
  return compare_operators.find (toktype) != compare_operators.end ();
}

bool
Parser::is_boolean_operator (TokenType toktype) const
{
  return boolean_operators.find (toktype) != boolean_operators.end ();
}

bool
Parser::is_pre_operator (TokenType toktype) const
{
  return pre_operators.find (toktype) != pre_operators.end ();
}

bool
Parser::is_post_operator (TokenType toktype) const
{
  return post_operators.find (toktype) != post_operators.end ();
}

bool
Parser::is_typemodifier (const std::string &str) const
{
  for (auto &mod : typemodifiers)
    {
      if (str == mod)
        return true;
    }
  return false;
}

bool
Parser::is_operand (TokenType toktype) const
{
  return is_idoperand (toktype) || toktype == TokenType::TOKEN_TYPE_NUMBER
         || toktype == TokenType::TOKEN_TYPE_FLOATING;
}

bool
Parser::is_idoperand (TokenType toktype) const
{
  return toktype == TokenType::TOKEN_TYPE_ID
         || toktype == TokenType::TOKEN_TYPE_PLUS_PLUS
         || toktype == TokenType::TOKEN_TYPE_MINUS_MINUS;
}
}

#pragma once

#include <cstddef>
#include <lexer.hpp>
#include <string>
#include <vector>

namespace nlc
{

enum CSTNodeType
{
  CST_NODE_TYPE_UNK = 0,

  CST_NODE_TYPE_PROGRAM,

  CST_NODE_TYPE_STMTLIST,

  CST_NODE_TYPE_STMT_IF,
  CST_NODE_TYPE_STMT_ELSE,
  CST_NODE_TYPE_STMT_SWITCH,
  CST_NODE_TYPE_STMT_DEREF,
  CST_NODE_TYPE_STMT_RETURN,

  CST_NODE_TYPE_IDSTMT_VAR_ASSIGN_TO_EXPR,
  CST_NODE_TYPE_IDSTMT_VAR_ASSIGN_TO_EXPRLIST,
  CST_NODE_TYPE_IDSTMT_ELEM_ASSIGN_TO_EXPR,
  CST_NODE_TYPE_IDSTMT_ELEM_ASSIGN_TO_EXPRLIST,
  CST_NODE_TYPE_IDSTMT_CALL,
  CST_NODE_TYPE_IDSTMT_LABEL,

  CST_NODE_TYPE_CASESTMT_CASE,
  CST_NODE_TYPE_CASESTMT_DEFAULT,

  CST_NODE_TYPE_EXPR,

  CST_NODE_TYPE_OP_ADD,
  CST_NODE_TYPE_OP_SUB,
  CST_NODE_TYPE_OP_MUL,
  CST_NODE_TYPE_OP_DIV,
  CST_NODE_TYPE_OP_MOD,
  CST_NODE_TYPE_OP_AND,
  CST_NODE_TYPE_OP_OR,
  CST_NODE_TYPE_OP_XOR,
  CST_NODE_TYPE_OP_BOOLNOT,
  CST_NODE_TYPE_OP_NOT,

  CST_NODE_TYPE_ASSIGNOP_EQUAL,
  CST_NODE_TYPE_ASSIGNOP_ADD_EQ,
  CST_NODE_TYPE_ASSIGNOP_SUB_EQ,
  CST_NODE_TYPE_ASSIGNOP_MUL_EQ,
  CST_NODE_TYPE_ASSIGNOP_DIV_EQ,
  CST_NODE_TYPE_ASSIGNOP_MOD_EQ,
  CST_NODE_TYPE_ASSIGNOP_AND_EQ,
  CST_NODE_TYPE_ASSIGNOP_OR_EQ,
  CST_NODE_TYPE_ASSIGNOP_XOR_EQ,

  CST_NODE_TYPE_CMPOP_EQUAL,
  CST_NODE_TYPE_CMPOP_NOT_EQUAL,
  CST_NODE_TYPE_CMPOP_LESS,
  CST_NODE_TYPE_CMPOP_GREATER,
  CST_NODE_TYPE_CMPOP_LESS_OR_EQUAL,
  CST_NODE_TYPE_CMPOP_GREATER_OR_EQUAL,

  CST_NODE_TYPE_BOOLOP_AND,
  CST_NODE_TYPE_BOOLOP_OR,
};

struct CSTNode
{
  CSTNodeType type;
  std::string value;
  std::vector<CSTNode> children;

  CSTNode () : type (CSTNodeType::CST_NODE_TYPE_UNK), value{}, children{} {}
  CSTNode (CSTNodeType type, const std::string &value = {})
      : type (type), value (value), children{}
  {
  }

  void
  append (CSTNode child)
  {
    children.push_back (child);
  }

  std::string to_string (size_t depth) const;
};

class Parser
{
public:
  Parser () = default;
  CSTNode parse (const std::vector<Token> &tokens);

private:
  const std::map<TokenType, CSTNodeType> operators = {
    { TokenType::TOKEN_TYPE_PLUS, CSTNodeType::CST_NODE_TYPE_OP_ADD },
    { TokenType::TOKEN_TYPE_MINUS, CSTNodeType::CST_NODE_TYPE_OP_SUB },
    { TokenType::TOKEN_TYPE_SLASH, CSTNodeType::CST_NODE_TYPE_OP_DIV },
    { TokenType::TOKEN_TYPE_ASTERISK, CSTNodeType::CST_NODE_TYPE_OP_MUL },
    { TokenType::TOKEN_TYPE_PERCENT, CSTNodeType::CST_NODE_TYPE_OP_MOD },
    { TokenType::TOKEN_TYPE_AMPERSAND, CSTNodeType::CST_NODE_TYPE_OP_AND },
    { TokenType::TOKEN_TYPE_PIPE, CSTNodeType::CST_NODE_TYPE_OP_OR },
    { TokenType::TOKEN_TYPE_CACCENT, CSTNodeType::CST_NODE_TYPE_OP_XOR },
    { TokenType::TOKEN_TYPE_EXCLMARK, CSTNodeType::CST_NODE_TYPE_OP_BOOLNOT },
    { TokenType::TOKEN_TYPE_TILDE, CSTNodeType::CST_NODE_TYPE_OP_NOT },
  };

  const std::map<TokenType, CSTNodeType> assign_operators = {
    { TokenType::TOKEN_TYPE_EQ_EQ, CSTNodeType::CST_NODE_TYPE_ASSIGNOP_EQUAL },
    { TokenType::TOKEN_TYPE_PLUS_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_ADD_EQ },
    { TokenType::TOKEN_TYPE_MINUS_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_SUB_EQ },
    { TokenType::TOKEN_TYPE_ASTERISK_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_MUL_EQ },
    { TokenType::TOKEN_TYPE_SLASH_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_DIV_EQ },
    { TokenType::TOKEN_TYPE_PERCENT_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_MOD_EQ },
    { TokenType::TOKEN_TYPE_ASTERISK_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_AND_EQ },
    { TokenType::TOKEN_TYPE_PIPE_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_OR_EQ },
    { TokenType::TOKEN_TYPE_CACCENT_EQ,
      CSTNodeType::CST_NODE_TYPE_ASSIGNOP_XOR_EQ },
  };

  const std::map<TokenType, CSTNodeType> compare_operators = {
    { TokenType::TOKEN_TYPE_EQ_EQ, CSTNodeType::CST_NODE_TYPE_CMPOP_EQUAL },
    { TokenType::TOKEN_TYPE_AMPERSAND_EQ,
      CSTNodeType::CST_NODE_TYPE_CMPOP_NOT_EQUAL },
    { TokenType::TOKEN_TYPE_LARROW, CSTNodeType::CST_NODE_TYPE_CMPOP_LESS },
    { TokenType::TOKEN_TYPE_RARROW, CSTNodeType::CST_NODE_TYPE_CMPOP_GREATER },
    { TokenType::TOKEN_TYPE_LARROW_EQ,
      CSTNodeType::CST_NODE_TYPE_CMPOP_LESS_OR_EQUAL },
    { TokenType::TOKEN_TYPE_RARROW_EQ,
      CSTNodeType::CST_NODE_TYPE_CMPOP_GREATER_OR_EQUAL },
  };

  const std::map<TokenType, CSTNodeType> boolean_operators = {
    { TokenType::TOKEN_TYPE_AMPERSAND_AMPERSAND,
      CSTNodeType::CST_NODE_TYPE_BOOLOP_AND },
    { TokenType::TOKEN_TYPE_PIPE_PIPE, CSTNodeType::CST_NODE_TYPE_BOOLOP_OR },
  };

  const std::vector<std::string> keywords = {
    "const",
    "static",
    "inline",
  };

  std::vector<Token> toks{};
  size_t pos{};
  size_t tokenslen{};

  // <stmtlist>
  //   : <stmt> <stmtlist>
  //   | <stmt>
  //   ;
  CSTNode parse_statement_list ();

  // <stmt>
  //   : <idstmt>
  //   | if (<expr>) <stmt>
  //   | else <stmt>
  //   | switch { <casestmts> }
  //   | return <expr>;
  //   | *<stmt>
  //   | { <stmtlist> }
  //   ;
  CSTNode parse_statement ();
  CSTNode parse_if_statement ();     // if (<expr>) <stmt>
  CSTNode parse_else_statement ();   // else <stmt>
  CSTNode parse_switch_statement (); // switch { <casestmts> }
  CSTNode parse_return_statement (); // return <expr>;

  // <idstmt>
  //   : <idcreatestmt>
  //   | <id> <assignop> <expr>;
  //   | <id> = { <exprlist> };
  //   | <arraystmt>
  //   | <call>;
  //   | <id>:
  //   ;
  CSTNode parse_identifier_statement ();

  // <casestmt>
  //   : case <expr>: { <stmtlist> }
  //   | default: { <stmtlist> }
  //   ;
  CSTNode parse_case_statement ();

  CSTNode parse_label_statement (); // <id>:

  // TODO: remove me
  void skip_until (TokenType toktype);

  bool is_operator (TokenType toktype) const;
  bool is_assign_operator (TokenType toktype) const;
  bool is_compare_operator (TokenType toktype) const;
  bool is_boolean_operator (TokenType toktype) const;
  bool is_keyword (const std::string &str) const;
};

}

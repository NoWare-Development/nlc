#pragma once

#include <cstddef>
#include <lexer.hpp>
#include <string>
#include <vector>

namespace nlc
{

enum CSTNodeType : unsigned char
{
  CST_NODE_TYPE_UNK = 0b0000'0000,
  CST_NODE_TYPE_PROGRAM = 0b0000'0001,
  CST_NODE_TYPE_STMTLIST = 0b0000'0010,
  CST_NODE_TYPE_EXPR = 0b0000'0011,

  CST_NODE_TYPE_STMT_IF = 0b0001'0000,
  CST_NODE_TYPE_STMT_ELSE = 0b0001'0001,
  CST_NODE_TYPE_STMT_SWITCH = 0b0001'0010,
  CST_NODE_TYPE_STMT_DEREF = 0b0001'0011,
  CST_NODE_TYPE_STMT_RETURN = 0b0001'0100,
  CST_NODE_TYPE_STMT_BREAK = 0b0001'0101,
  CST_NODE_TYPE_STMT_CONTINUE = 0b0001'0110,

  CST_NODE_TYPE_IDSTMT_VAR_ASSIGN_TO_EXPR = 0b0010'0000,
  CST_NODE_TYPE_IDSTMT_VAR_ASSIGN_TO_EXPRLIST = 0b0010'0001,
  CST_NODE_TYPE_IDSTMT_ELEM_ASSIGN_TO_EXPR = 0b0010'0010,
  CST_NODE_TYPE_IDSTMT_ELEM_ASSIGN_TO_EXPRLIST = 0b0010'0011,
  CST_NODE_TYPE_IDSTMT_CALL = 0b0010'0100,
  CST_NODE_TYPE_IDSTMT_LABEL = 0b0010'0101,

  CST_NODE_TYPE_CASESTMT_CASE = 0b0011'0000,
  CST_NODE_TYPE_CASESTMT_DEFAULT = 0b0011'0001,

  CST_NODE_TYPE_OP_ADD = 0b0100'0000,
  CST_NODE_TYPE_OP_SUB = 0b0100'0001,
  CST_NODE_TYPE_OP_MUL = 0b0100'0010,
  CST_NODE_TYPE_OP_DIV = 0b0100'0011,
  CST_NODE_TYPE_OP_MOD = 0b0100'0100,
  CST_NODE_TYPE_OP_AND = 0b0100'0101,
  CST_NODE_TYPE_OP_OR = 0b0100'0110,
  CST_NODE_TYPE_OP_XOR = 0b0100'0111,

  CST_NODE_TYPE_PREFIXOP_BOOLNOT = 0b0101'0000,
  CST_NODE_TYPE_PREFIXOP_MINUS = 0b0101'0001,
  CST_NODE_TYPE_PREFIXOP_NOT = 0b0101'0010,

  CST_NODE_TYPE_ASSIGNOP_EQUAL = 0b0110'0000,
  CST_NODE_TYPE_ASSIGNOP_ADD_EQ = 0b0110'0001,
  CST_NODE_TYPE_ASSIGNOP_SUB_EQ = 0b0110'0010,
  CST_NODE_TYPE_ASSIGNOP_MUL_EQ = 0b0110'0011,
  CST_NODE_TYPE_ASSIGNOP_DIV_EQ = 0b0110'0100,
  CST_NODE_TYPE_ASSIGNOP_MOD_EQ = 0b0110'0101,
  CST_NODE_TYPE_ASSIGNOP_AND_EQ = 0b0110'0110,
  CST_NODE_TYPE_ASSIGNOP_OR_EQ = 0b0110'0111,
  CST_NODE_TYPE_ASSIGNOP_XOR_EQ = 0b0110'1000,

  CST_NODE_TYPE_CMPOP_EQUAL = 0b0111'0000,
  CST_NODE_TYPE_CMPOP_NOT_EQUAL = 0b0111'0001,
  CST_NODE_TYPE_CMPOP_LESS = 0b0111'0010,
  CST_NODE_TYPE_CMPOP_GREATER = 0b0111'0011,
  CST_NODE_TYPE_CMPOP_LESS_OR_EQUAL = 0b0111'0100,
  CST_NODE_TYPE_CMPOP_GREATER_OR_EQUAL = 0b0111'0101,

  CST_NODE_TYPE_BOOLOP_AND = 0b1000'0000,
  CST_NODE_TYPE_BOOLOP_OR = 0b1000'0001,

  CST_NODE_TYPE_PREOP_INCREMENT = 0b1001'0000,
  CST_NODE_TYPE_PREOP_DECREMENT = 0b1001'0001,

  CST_NODE_TYPE_POSTOP_INCREMENT = 0b1010'0000,
  CST_NODE_TYPE_POSTOP_DECREMENT = 0b1010'0001,

  CST_NODE_TYPE_OPERAND_INTEGER = 0b1011'0000,
  CST_NODE_TYPE_OPERAND_FLOATING = 0b1011'0001,
  CST_NODE_TYPE_OPERAND_ID = 0b1011'0010,
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

  void append (CSTNode child);

  std::string to_string () const;

private:
  size_t depth{ 0 };

  void change_depth (size_t depth);
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
  };

  const std::map<TokenType, CSTNodeType> prefix_operators = {
    { TokenType::TOKEN_TYPE_EXCLMARK,
      CSTNodeType::CST_NODE_TYPE_PREFIXOP_BOOLNOT },
    { TokenType::TOKEN_TYPE_MINUS, CSTNodeType::CST_NODE_TYPE_PREFIXOP_MINUS },
    { TokenType::TOKEN_TYPE_TILDE, CSTNodeType::CST_NODE_TYPE_PREFIXOP_NOT },
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

  const std::map<TokenType, CSTNodeType> pre_operators = {
    { TokenType::TOKEN_TYPE_PLUS_PLUS,
      CSTNodeType::CST_NODE_TYPE_PREOP_INCREMENT },
    { TokenType::TOKEN_TYPE_MINUS_MINUS,
      CSTNodeType::CST_NODE_TYPE_PREOP_DECREMENT },
  };

  const std::map<TokenType, CSTNodeType> post_operators = {
    { TokenType::TOKEN_TYPE_PLUS_PLUS,
      CSTNodeType::CST_NODE_TYPE_POSTOP_INCREMENT },
    { TokenType::TOKEN_TYPE_MINUS_MINUS,
      CSTNodeType::CST_NODE_TYPE_POSTOP_DECREMENT },
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
  //   | break;
  //   | continue;
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

  CSTNode parse_expression ();
  CSTNode parse_prefix_operator ();
  CSTNode parse_operand ();
  CSTNode parse_idoperand ();

  CSTNode parse_label_statement (); // <id>:

  // TODO: remove me
  void skip_until (TokenType toktype);

  bool is_operator (TokenType toktype) const;
  bool is_prefix_operator (TokenType toktype) const;
  bool is_assign_operator (TokenType toktype) const;
  bool is_compare_operator (TokenType toktype) const;
  bool is_boolean_operator (TokenType toktype) const;
  bool is_pre_operator (TokenType toktype) const;
  bool is_post_operator (TokenType toktype) const;
  bool is_keyword (const std::string &str) const;
  bool is_operand (TokenType toktype) const;
  bool is_idoperand (TokenType toktype) const;
};

}

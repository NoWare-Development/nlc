#pragma once

#include <cstddef>
#include <lexer.hpp>
#include <string>
#include <vector>

namespace nlc
{

// 0b00000000'00000000
//   ~~~~~~~^ ~~~~~~~^
//      Group     Type
#define __NLC_CSTNODETYPE_DEF_(group, id)                                     \
  (((group) & 0xFF) << 8) | ((id) & 0xFF)
#define __NLC_CSTNODETYPE_GET_GROUP_(type) (((type) >> 8) & 0xFF)
#define __NLC_CSTNODETYPE_GET_ID_(type) ((type) & 0xFF)

enum CSTNodeType : unsigned short
{
  CST_NODE_TYPE_UNK = __NLC_CSTNODETYPE_DEF_ (0, 0),
  CST_NODE_TYPE_PROGRAM = __NLC_CSTNODETYPE_DEF_ (0, 1),
  CST_NODE_TYPE_STMTLIST = __NLC_CSTNODETYPE_DEF_ (0, 2),
  CST_NODE_TYPE_EXPR = __NLC_CSTNODETYPE_DEF_ (0, 3),
  CST_NODE_TYPE_TYPEMODIFIER = __NLC_CSTNODETYPE_DEF_ (0, 4),
  CST_NODE_TYPE_FUNCTION_PROTOTYPE = __NLC_CSTNODETYPE_DEF_ (0, 5),

  CST_NODE_TYPE_STMT_IF = __NLC_CSTNODETYPE_DEF_ (1, 0),
  CST_NODE_TYPE_STMT_ELSE = __NLC_CSTNODETYPE_DEF_ (1, 1),
  CST_NODE_TYPE_STMT_SWITCH = __NLC_CSTNODETYPE_DEF_ (1, 2),
  CST_NODE_TYPE_STMT_RETURN = __NLC_CSTNODETYPE_DEF_ (1, 3),
  CST_NODE_TYPE_STMT_BREAK = __NLC_CSTNODETYPE_DEF_ (1, 4),
  CST_NODE_TYPE_STMT_CONTINUE = __NLC_CSTNODETYPE_DEF_ (1, 5),
  CST_NODE_TYPE_STMT_WHILE = __NLC_CSTNODETYPE_DEF_ (1, 6),
  CST_NODE_TYPE_STMT_FOR = __NLC_CSTNODETYPE_DEF_ (1, 7),
  CST_NODE_TYPE_STMT_GOTO = __NLC_CSTNODETYPE_DEF_ (1, 8),
  CST_NODE_TYPE_STMT_LABEL = __NLC_CSTNODETYPE_DEF_ (1, 9),

  CST_NODE_TYPE_IDSTMT_CREATE = __NLC_CSTNODETYPE_DEF_ (2, 0),
  CST_NODE_TYPE_IDSTMT_ASSIGN = __NLC_CSTNODETYPE_DEF_ (2, 1),
  CST_NODE_TYPE_IDSTMT_FUNC_DECLARATION = __NLC_CSTNODETYPE_DEF_ (2, 2),
  CST_NODE_TYPE_IDSTMT_FUNC_DEFINITION = __NLC_CSTNODETYPE_DEF_ (2, 3),
  CST_NODE_TYPE_IDSTMT_CALL = __NLC_CSTNODETYPE_DEF_ (2, 4),

  CST_NODE_TYPE_CASESTMT_CASE = __NLC_CSTNODETYPE_DEF_ (3, 0),
  CST_NODE_TYPE_CASESTMT_DEFAULT = __NLC_CSTNODETYPE_DEF_ (3, 1),

  CST_NODE_TYPE_OP_ADD = __NLC_CSTNODETYPE_DEF_ (4, 0),
  CST_NODE_TYPE_OP_SUB = __NLC_CSTNODETYPE_DEF_ (4, 1),
  CST_NODE_TYPE_OP_MUL = __NLC_CSTNODETYPE_DEF_ (4, 2),
  CST_NODE_TYPE_OP_DIV = __NLC_CSTNODETYPE_DEF_ (4, 3),
  CST_NODE_TYPE_OP_MOD = __NLC_CSTNODETYPE_DEF_ (4, 4),
  CST_NODE_TYPE_OP_AND = __NLC_CSTNODETYPE_DEF_ (4, 5),
  CST_NODE_TYPE_OP_OR = __NLC_CSTNODETYPE_DEF_ (4, 6),
  CST_NODE_TYPE_OP_XOR = __NLC_CSTNODETYPE_DEF_ (4, 7),

  CST_NODE_TYPE_PREFIXOP_BOOLNOT = __NLC_CSTNODETYPE_DEF_ (5, 0),
  CST_NODE_TYPE_PREFIXOP_MINUS = __NLC_CSTNODETYPE_DEF_ (5, 1),
  CST_NODE_TYPE_PREFIXOP_NOT = __NLC_CSTNODETYPE_DEF_ (5, 2),

  CST_NODE_TYPE_ASSIGNOP_EQUAL = __NLC_CSTNODETYPE_DEF_ (6, 0),
  CST_NODE_TYPE_ASSIGNOP_ADD_EQ = __NLC_CSTNODETYPE_DEF_ (6, 1),
  CST_NODE_TYPE_ASSIGNOP_SUB_EQ = __NLC_CSTNODETYPE_DEF_ (6, 2),
  CST_NODE_TYPE_ASSIGNOP_MUL_EQ = __NLC_CSTNODETYPE_DEF_ (6, 3),
  CST_NODE_TYPE_ASSIGNOP_DIV_EQ = __NLC_CSTNODETYPE_DEF_ (6, 4),
  CST_NODE_TYPE_ASSIGNOP_MOD_EQ = __NLC_CSTNODETYPE_DEF_ (6, 5),
  CST_NODE_TYPE_ASSIGNOP_AND_EQ = __NLC_CSTNODETYPE_DEF_ (6, 6),
  CST_NODE_TYPE_ASSIGNOP_OR_EQ = __NLC_CSTNODETYPE_DEF_ (6, 7),
  CST_NODE_TYPE_ASSIGNOP_XOR_EQ = __NLC_CSTNODETYPE_DEF_ (6, 8),

  CST_NODE_TYPE_CMPOP_EQUAL = __NLC_CSTNODETYPE_DEF_ (7, 0),
  CST_NODE_TYPE_CMPOP_NOT_EQUAL = __NLC_CSTNODETYPE_DEF_ (7, 1),
  CST_NODE_TYPE_CMPOP_LESS = __NLC_CSTNODETYPE_DEF_ (7, 2),
  CST_NODE_TYPE_CMPOP_GREATER = __NLC_CSTNODETYPE_DEF_ (7, 3),
  CST_NODE_TYPE_CMPOP_LESS_OR_EQUAL = __NLC_CSTNODETYPE_DEF_ (7, 4),
  CST_NODE_TYPE_CMPOP_GREATER_OR_EQUAL = __NLC_CSTNODETYPE_DEF_ (7, 5),

  CST_NODE_TYPE_BOOLOP_AND = __NLC_CSTNODETYPE_DEF_ (8, 0),
  CST_NODE_TYPE_BOOLOP_OR = __NLC_CSTNODETYPE_DEF_ (8, 1),

  CST_NODE_TYPE_PREOP_INCREMENT = __NLC_CSTNODETYPE_DEF_ (9, 0),
  CST_NODE_TYPE_PREOP_DECREMENT = __NLC_CSTNODETYPE_DEF_ (9, 1),

  CST_NODE_TYPE_POSTOP_INCREMENT = __NLC_CSTNODETYPE_DEF_ (10, 0),
  CST_NODE_TYPE_POSTOP_DECREMENT = __NLC_CSTNODETYPE_DEF_ (10, 1),

  CST_NODE_TYPE_OPERAND_INTEGER = __NLC_CSTNODETYPE_DEF_ (11, 0),
  CST_NODE_TYPE_OPERAND_FLOATING = __NLC_CSTNODETYPE_DEF_ (11, 1),
  CST_NODE_TYPE_OPERAND_ID = __NLC_CSTNODETYPE_DEF_ (11, 2),

  CST_NODE_TYPE_IDCREATESTMT_WITH_ASSIGN = __NLC_CSTNODETYPE_DEF_ (12, 0),
  CST_NODE_TYPE_IDCREATESTMT_WITHOUT_ASSIGN = __NLC_CSTNODETYPE_DEF_ (12, 1),

  CST_NODE_TYPE_IDCREATE_WITH_MOD = __NLC_CSTNODETYPE_DEF_ (13, 0),
  CST_NODE_TYPE_IDCREATE_PLAIN = __NLC_CSTNODETYPE_DEF_ (13, 1),
  CST_NODE_TYPE_IDCREATE_LIST = __NLC_CSTNODETYPE_DEF_ (13, 2),

  CST_NODE_TYPE_IDTYPE_POINTER = __NLC_CSTNODETYPE_DEF_ (14, 0),
  CST_NODE_TYPE_IDTYPE_PLAIN = __NLC_CSTNODETYPE_DEF_ (14, 1),

  CST_NODE_TYPE_IDTYPERAW_PLAIN = __NLC_CSTNODETYPE_DEF_ (15, 0),
  CST_NODE_TYPE_IDTYPERAW_ARRAY = __NLC_CSTNODETYPE_DEF_ (15, 1),
  CST_NODE_TYPE_IDTYPERAW_FUNCPTR = __NLC_CSTNODETYPE_DEF_ (15, 2),
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

  const std::vector<std::string> typemodifiers = {
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
  //   | switch (<expr>) { <casestmts> }
  //   | return <expr>;
  //   | { <stmtlist> }
  //   | break;
  //   | continue;
  //   | while (<expr>) <stmt>
  //   | for (<stmt>; <expr>; <expr>) <stmt>
  //   | goto @<id>;
  //   | @<id>:
  //   ;
  CSTNode parse_statement ();
  CSTNode parse_if_statement ();     // if (<expr>) <stmt>
  CSTNode parse_else_statement ();   // else <stmt>
  CSTNode parse_switch_statement (); // switch (<expr>) { <casestmts> }
  CSTNode parse_return_statement (); // return <expr>;
  CSTNode parse_while_statement ();  // while (<expr>) <stmt>
  CSTNode parse_for_statement ();    // for (<stmt>; <expr>; <expr>) <stmt>
  CSTNode parse_goto_statement ();   // goto @<id>;
  CSTNode parse_label_statement ();  // @<id>:

  // <idstmt>
  //   : <idcreatestmt>
  //   | <idoperand> <assignop> <expr>;
  //   | <funcproto>;
  //   | <funcproto> { <stmtlist> }
  //   | <call>;
  //   ;
  CSTNode parse_identifier_statement ();
  CSTNode parse_function_prototype_statement ();

  // <idcreatestmt>
  //   : <idcreate> = <expr>;
  //   | <idcreate>;
  //   ;
  CSTNode parse_identifier_create_statement ();

  // <idcreate>
  //   : <typemodifier> <idcreate>
  //   | <id>: <idtype>
  //   ;
  CSTNode parse_identifier_create ();
  CSTNode parse_identifier_create_list ();

  // <idtype>
  //   : *<idtype>
  //   | <idtyperaw>
  //   ;
  CSTNode parse_identifier_type ();

  // <idtyperaw>
  //   : <id>
  //   | <id>[<expr>]
  //   | (<idcreatelist>) -> <id>
  //   ;
  CSTNode parse_identifier_type_raw ();
  CSTNode parse_identifier_type_raw_plain (); // <id>
  CSTNode parse_identifier_type_raw_array (); // <id>[<expr>]
  CSTNode
  parse_identifier_type_raw_funcptr (); // <idtyperaw> :: (<idcreatelist>)

  // <funcproto>
  //   : <id> :: (<idcreatelist>) -> <id>
  //   ;
  CSTNode parse_function_prototype ();

  // <call>
  //   : <id>(<exprlist>)
  //   ;
  CSTNode parse_call ();

  // <casestmt>
  //   : case <expr>: { <stmtlist> }
  //   | default: { <stmtlist> }
  //   ;
  CSTNode parse_case_statement ();

  CSTNode parse_expression ();
  CSTNode parse_prefix_operator ();
  CSTNode parse_operand ();
  CSTNode parse_idoperand ();

  // TODO: remove me
  void skip_until (TokenType toktype);

  bool is_operator (TokenType toktype) const;
  bool is_prefix_operator (TokenType toktype) const;
  bool is_assign_operator (TokenType toktype) const;
  bool is_compare_operator (TokenType toktype) const;
  bool is_boolean_operator (TokenType toktype) const;
  bool is_pre_operator (TokenType toktype) const;
  bool is_post_operator (TokenType toktype) const;
  bool is_typemodifier (const std::string &str) const;
  bool is_operand (TokenType toktype) const;
  bool is_idoperand (TokenType toktype) const;
};

}

#pragma once

#include <ast.hpp>
#include <cstdint>
#include <lexer.hpp>
#include <map>
#include <vector>

namespace nlc
{

class Parser
{
public:
  Parser (const std::vector<Token> &tokens) : _tokens{ tokens } {}

  struct ParserError
  {
    enum ErrType
    {
      PARSER_ERROR_UNK,
      PARSER_ERROR_EXPECTED,
      PARSER_ERROR_UNEXPECTED,
      PARSER_ERROR_INVALID_EXPR,
    };

    ErrType type;
    size_t pos;
    size_t len;

    ParserError (ErrType type, size_t pos, size_t len = 1)
        : type{ type }, pos{ pos }, len{ len }
    {
    }
  };

  AST parse ();

  const std::vector<ParserError> &
  get_errors () const
  {
    return _errors;
  }

private:
  std::vector<Token> _tokens;
  std::vector<ParserError> _errors{};
  size_t _pos{};
  bool _errored = false;

  // <top(export)>
  //   : <decldef>
  //   | <typedef>
  //   | <structdef>
  //   | <enumdef>
  //   | <template>
  //   !export
  //   :
  //   | <import>
  //   | <export>
  //   ;
  AST parse_top (bool for_export = false);

  // <typedef>
  //   : typedef <type> <id>;
  //   ;
  AST parse_typedef ();

  // <type>
  //   : <typeraw>
  //   | <typeraw>[<expr>]
  //   ;
  AST parse_type ();

  // <typeraw>
  //   : *<typeraw>
  //   | <id>
  //   ;
  AST parse_type_raw ();

  // <enumdef>
  //   : enum <id> { <enumelements> }
  //   ;
  AST parse_enum_definition ();
  AST parse_enum_element ();

  // <import>
  //   : import <module>;
  //   ;
  AST parse_import ();

  // <export>
  //   : export <top(export)>
  //   ;
  AST parse_export ();

  // <module>
  //   : <id>.<module>
  //   | <id>
  //   ;
  AST parse_module ();

  // <decldef>
  //   : <modifier> <decldef>
  //   | <vardecl>;
  //   | <vardef>;
  //   | <funcdecl>
  //   | <funcdef>
  //   ;
  AST parse_decldef ();

  // <vardecl>
  //   : <id>: <type>
  //   ;
  //
  // <vardef>
  //   : <id>: <type> = <expr>
  //   ;
  AST parse_variable_decldef ();

  // <funcdecl>
  //   : <funcproto>;
  //   ;
  //
  // <funcdef>
  //   : <funcproto> <stmtlist>
  //   ;
  AST parse_function_decldef ();

  // <funcproto>
  //   : <id> :: (<funcargs>) -> <type>
  //   ;
  AST parse_function_prototype ();

  // <funcarg>
  //   : <vardecl>
  //   | <vardef>
  //   | ...
  //   ;
  AST parse_function_argument ();

  // <stmt>
  //   : ;
  //   | <exprstmt>
  //   | <returnstmt>
  //   | <gotostmt>
  //   | <labelstmt>
  //   | <idstmt>
  //   | <whilestmt>
  //   | <forstmt>
  //   | <dowhilestmt>
  //   | <switchstmt>
  //   | <deferstmt>
  //   | <structdef>
  //   | <template>
  //   | break;
  //   | continue;
  //   ;
  AST parse_statement ();
  AST parse_expression_statement (); // <exprstmt>: <expr>;

  // <stmtlist>
  //   : <stmt> <stmtlist>
  //   | <stmt>
  //   ;
  AST parse_statement_list ();

  // <returnstmt>
  //   : return <expr>;
  //   ;
  AST parse_return_statement ();

  // <gotostmt>
  //   : goto <label>;
  //   ;
  AST parse_goto_statement ();

  // <labelstmt>
  //   : <label>:
  //   ;
  AST parse_label_statement ();

  // <label>
  //   : @<id>
  //   ;
  AST parse_label ();

  // <idstmt>
  AST parse_identifier_statement ();

  // <whilestmt>
  //   : while (<expr>) <stmt>
  //   ;
  AST parse_while_statement ();

  // <forstmt>
  //   : for (<stmt> <expr>; <expr>)
  //   ;
  AST parse_for_statement ();

  // <dowhilestmt>
  //   : do <stmt> while (<expr>);
  //   ;
  AST parse_do_while_statement ();

  // <switchstmt>
  //   : switch (<expr>) { <casestmts> }
  //   ;
  AST parse_switch_statement ();

  // <casestmt>
  //   : case <expr>: <stmt>
  //   ;
  AST parse_case_statement ();

  // <deferstmt>
  //   : defer <stmt>
  //   ;
  AST parse_defer_statement ();

  // <structdef>
  //   : struct <id> { <decldefs> }
  //   ;
  AST parse_struct ();

  // <template>
  //   : template <ids> <templated>
  //   ;
  // <templated>
  //   : <structdef>
  //   | <funcdecl>
  //   | <funcdef>
  //   ;
  AST parse_template ();

  // <vismarker>
  //   : public:
  //   | private:
  //   ;
  AST parse_visibility_marker ();

  // <initlist>
  //   : { <initlistentries> }
  //   ;
  AST parse_initialization_list ();

  // <initlistentry>
  //   : <expr>
  //   | .<id> = <expr>
  //   ;
  AST parse_initialization_list_entry ();

  AST parse_break_statement ();    // break;
  AST parse_continue_statement (); // continue;

  // <expr>
  // NOTE: `toplevel` is passed only in expression statements.
  // It allows to use assignment operators in the expression.
  AST parse_expression (bool toplevel = false);
  AST parse_expression_tree (bool toplevel = false);
  AST parse_expression_operand ();
  AST parse_call_operand ();
  AST pratt_parse_expression (const std::vector<AST> &in, size_t *pos,
                              int min_bp) const;
  bool validate_expression (const AST &expr_ast, size_t &invalid_pos,
                            std::string &out_reason) const;
  void get_binding_power (ASTType op_type, int &l_bp, int &r_bp) const;

  AST number_to_operand (Token &tok) const;
  uint64_t string_to_dec (const std::string &str) const;
  uint64_t string_to_hex (const std::string &str) const;
  uint64_t string_to_bin (const std::string &str) const;
  uint64_t string_to_oct (const std::string &str) const;
  std::string hex_to_string (uint64_t val) const;

  void skip_until (TokenType type);

  bool verify_pos (size_t pos);
  bool verify_token (size_t pos, TokenType got, TokenType expected);
  bool verify_value (size_t pos, const std::string &got,
                     const std::string &expected);

  TokenType peek (size_t pos) const;

  bool is_modifier (const std::string &str) const;

  bool is_operator (TokenType type) const;
  bool is_binary_operator (TokenType type) const;
  bool is_assign_operator (TokenType type) const;
  bool is_compare_operator (TokenType type) const;
  bool is_prefix_operator (TokenType type) const;
  bool is_numeric_token (TokenType type) const;

  bool is_operator (ASTType type) const;
  bool is_binary_operator (ASTType type) const;
  bool is_assign_operator (ASTType type) const;
  bool is_compare_operator (ASTType type) const;
  bool is_prefix_operator (ASTType type) const;

  bool is_operand (ASTType type) const;

  template <typename... Args>
  void
  add_error (Args &&...args)
  {
    _errored = true;
    _errors.emplace_back (args...);
  }

  const std::map<TokenType, ASTType> _binary_operators = {
    { TokenType::TOKEN_ADD, ASTType::AST_EXPR_BINARY_OPERATOR_ADD },
    { TokenType::TOKEN_SUB, ASTType::AST_EXPR_BINARY_OPERATOR_SUB },
    { TokenType::TOKEN_MUL, ASTType::AST_EXPR_BINARY_OPERATOR_MUL },
    { TokenType::TOKEN_DIV, ASTType::AST_EXPR_BINARY_OPERATOR_DIV },
    { TokenType::TOKEN_MOD, ASTType::AST_EXPR_BINARY_OPERATOR_MOD },
    { TokenType::TOKEN_SHL, ASTType::AST_EXPR_BINARY_OPERATOR_SHL },
    { TokenType::TOKEN_SHR, ASTType::AST_EXPR_BINARY_OPERATOR_SHR },
    { TokenType::TOKEN_BAND, ASTType::AST_EXPR_BINARY_OPERATOR_AND },
    { TokenType::TOKEN_BOR, ASTType::AST_EXPR_BINARY_OPERATOR_OR },
    { TokenType::TOKEN_BXOR, ASTType::AST_EXPR_BINARY_OPERATOR_XOR },
  };

  const std::map<TokenType, ASTType> _compare_operators = {
    { TokenType::TOKEN_EQEQ, ASTType::AST_EXPR_COMPARE_OPERATOR_EQ },
    { TokenType::TOKEN_NOTEQ, ASTType::AST_EXPR_COMPARE_OPERATOR_NOTEQ },
    { TokenType::TOKEN_LTHAN, ASTType::AST_EXPR_COMPARE_OPERATOR_LTHAN },
    { TokenType::TOKEN_GTHAN, ASTType::AST_EXPR_COMPARE_OPERATOR_GTHAN },
    { TokenType::TOKEN_LTHANEQ, ASTType::AST_EXPR_COMPARE_OPERATOR_LTHANEQ },
    { TokenType::TOKEN_GTHANEQ, ASTType::AST_EXPR_COMPARE_OPERATOR_GTHANEQ },
  };

  const std::map<TokenType, ASTType> _assign_operators = {
    { TokenType::TOKEN_EQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_EQ },
    { TokenType::TOKEN_ADDEQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_ADDEQ },
    { TokenType::TOKEN_SUBEQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_SUBEQ },
    { TokenType::TOKEN_MULEQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_MULEQ },
    { TokenType::TOKEN_DIVEQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_DIVEQ },
    { TokenType::TOKEN_MODEQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_MODEQ },
    { TokenType::TOKEN_SHLEQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_SHLEQ },
    { TokenType::TOKEN_SHREQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_SHREQ },
    { TokenType::TOKEN_BANDEQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_ANDEQ },
    { TokenType::TOKEN_BOREQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_OREQ },
    { TokenType::TOKEN_BXOREQ, ASTType::AST_EXPR_ASSIGN_OPERATOR_XOREQ },
  };

  const std::map<TokenType, ASTType> _prefix_operators = {
    { TokenType::TOKEN_NOT, ASTType::AST_EXPR_PREFIX_OPERATOR_NOT },
    { TokenType::TOKEN_BNOT, ASTType::AST_EXPR_PREFIX_OPERATOR_BNOT },
    { TokenType::TOKEN_SUB, ASTType::AST_EXPR_PREFIX_OPERATOR_NEG },
    { TokenType::TOKEN_MUL, ASTType::AST_EXPR_PREFIX_OPERATOR_DEREF },
  };

  const std::vector<TokenType> _numeric_tokens = {
    TokenType::TOKEN_NUM,      TokenType::TOKEN_NUMHEX,
    TokenType::TOKEN_NUMBIN,   TokenType::TOKEN_NUMOCT,
    TokenType::TOKEN_NUMFLOAT,
  };

  const std::vector<std::string> _modifiers = {
    "const",
    "inline",
    "static",
  };
};

}

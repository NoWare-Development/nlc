#pragma once

#include "lexer.hpp"
#include <string>
#include <vector>

namespace nlc
{

#define __NLC_CST_TYPE_DEFINE_(group, index)                                  \
  ((((group) & 0xFF) << 8) | ((index) & 0xFF))
#define __NLC_CST_TYPE_GROUP_(type) (((type) >> 8) & 0xFF)
#define __NLC_CST_TYPE_INDEX_(type) ((type) & 0xFF)

enum CSTType : unsigned short
{
  CST_PROG = __NLC_CST_TYPE_DEFINE_ (0, 0),
  CST_STMTLIST = __NLC_CST_TYPE_DEFINE_ (0, 1),
  CST_MODULE = __NLC_CST_TYPE_DEFINE_ (0, 2),

  CST_STMT_RETURN = __NLC_CST_TYPE_DEFINE_ (1, 0),
  CST_STMT_IF = __NLC_CST_TYPE_DEFINE_ (1, 1),
  CST_STMT_ELSE = __NLC_CST_TYPE_DEFINE_ (1, 2),
  CST_STMT_SWITCH = __NLC_CST_TYPE_DEFINE_ (1, 3),
  CST_STMT_GOTO = __NLC_CST_TYPE_DEFINE_ (1, 4),
  CST_STMT_BREAK = __NLC_CST_TYPE_DEFINE_ (1, 5),
  CST_STMT_CONTINUE = __NLC_CST_TYPE_DEFINE_ (1, 6),
  CST_STMT_FOR = __NLC_CST_TYPE_DEFINE_ (1, 7),
  CST_STMT_WHILE = __NLC_CST_TYPE_DEFINE_ (1, 8),
  CST_STMT_LABEL = __NLC_CST_TYPE_DEFINE_ (1, 9),
  CST_STMT_IMPORT = __NLC_CST_TYPE_DEFINE_ (1, 10),
};

struct CST
{
  CSTType type;
  std::string value;
  size_t depth = 0;
  std::vector<CST> children{};

  CST () = default;
  CST (CSTType type, const std::string &value = "")
      : type{ type }, value{ value }
  {
  }

  void append (CST child);
  void change_depth (size_t depth);

  std::string to_string () const;
};

class Parser
{
public:
  enum ParserErrorType
  {
    PARSER_ERROR_UNK,
    PARSER_ERROR_EXPECTED,
    PARSER_ERROR_UNEXPECTED,
  };
  struct ParserError
  {
    std::string msg;
    size_t pos;
    ParserErrorType type;

    ParserError () = default;
    ParserError (size_t pos, ParserErrorType type, const std::string &msg = "")
        : msg{ msg }, pos{ pos }, type{ type }
    {
    }
  };

  Parser (const std::vector<Token> &tokens) : _tokens (tokens) {}

  CST parse ();

  std::vector<ParserError> get_errors () const;

private:
  std::vector<ParserError> _errors{};
  std::vector<Token> _tokens;
  size_t _errored = false;
  size_t _pos{};

  void skip_until (TokenType type);

  // <stmt>
  //   : { <stmtlist> }
  //   | <idcreatestmt>
  //   | return <expr>;
  //   | if (<expr>) <stmt>
  //   | else <stmt>
  //   | switch (<expr>) { <casestmtlist> }
  //   | goto @<id>;
  //   | break;
  //   | continue;
  //   | for (<stmt>; <expr>; <expr>) <stmt>
  //   | while (<expr>) <stmt>
  //   | @<label>:
  //   | import <module>;
  //   ;
  CST parse_statement ();
  CST parse_goto_statement ();   // goto @<id>;
  CST parse_return_statement (); // return <expr>;
  CST parse_label_statement ();  // @<label>:
  CST parse_import_statement (); // import <module>;

  // <module>
  //   : <id>.<module>
  //   | <id>
  //   ;
  CST parse_module ();

  void add_error (const ParserError &err);

  bool verify_pos (size_t pos);
  bool verify_tokentype (size_t pos, TokenType got, TokenType expected);
};

}

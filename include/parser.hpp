#pragma once

#include <lexer.hpp>
#include <string>
#include <vector>

namespace nlc
{

#define __CST_TYPE_DEFINE_(group, index)                                      \
  ((((group) & 0xFF) << 8) | ((index) & 0xFF))
enum CSTType : unsigned short
{
  CST_PROG = __CST_TYPE_DEFINE_ (0, 0),
  CST_STMTLIST = __CST_TYPE_DEFINE_ (0, 1),
  CST_IMPORT = __CST_TYPE_DEFINE_ (0, 2),
  CST_MACRO = __CST_TYPE_DEFINE_ (0, 3),
  CST_EXPR = __CST_TYPE_DEFINE_ (0, 4),

  CST_DECL_VAR_DECL = __CST_TYPE_DEFINE_ (1, 0),

  CST_STMT_RETURN = __CST_TYPE_DEFINE_ (2, 0),

  CST_MODULE_NAME = __CST_TYPE_DEFINE_ (3, 0),
  CST_MODULE_ITEM = __CST_TYPE_DEFINE_ (3, 1),
  CST_MODULE_EVERYTHING = __CST_TYPE_DEFINE_ (3, 2),
};
#undef __CST_TYPE_DEFINE_

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
  struct ParserWarn : ParserError
  {
  };

  Parser (const std::vector<Token> &tokens) : _tokens (tokens) {}

  CST parse ();

  std::vector<ParserError> get_errors () const;

private:
  std::vector<ParserError> _errors{};
  std::vector<Token> _tokens;
  bool _errored = false;
  size_t _pos{};

  // <entry>
  //   : <decl>
  //   | <macro>
  //   | <import>
  //   ;
  CST parse_entry ();

  // <import>
  //   : import <module>;
  //   ;
  CST parse_import ();

  // <module>
  //   : <id>.<module>
  //   | <id>
  //   ;
  CST parse_module ();

  void skip_until (TokenType type);

  template <typename... Args>
  void
  add_error (Args &&...args)
  {
    _errors.emplace_back (args...);
    _errored = true;
  }

  bool verify_pos (size_t pos);
  bool verify_tokentype (size_t pos, TokenType got, TokenType expected);

  TokenType peek (size_t pos);

#define __VERIFY_POS(pos)                                                     \
  {                                                                           \
    if (!verify_pos ((pos)))                                                  \
      {                                                                       \
        return {};                                                            \
      }                                                                       \
  }

#define __VERIFY_TOKENTYPE(pos, got, expected)                                \
  {                                                                           \
    if (!verify_tokentype ((pos), (got), (expected)))                         \
      {                                                                       \
        return {};                                                            \
      }                                                                       \
  }
};

}

#pragma once

#include <cstddef>
#include <string>
#include <vector>
namespace nlc
{

enum TokenType
{
  TOKEN_ERR = 0,

  TOKEN_ID,

  TOKEN_NUM,
  TOKEN_NUMHEX,
  TOKEN_NUMBIN,
  TOKEN_NUMOCT,
  TOKEN_NUMFLOAT,

  TOKEN_STRING,
  TOKEN_SYMBOL,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACK,
  TOKEN_RBRACK,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  TOKEN_LARROW,
  TOKEN_RARROW,

  TOKEN_LTHAN,
  TOKEN_GTHAN,

  TOKEN_COLON,
  TOKEN_DCOLON,

  TOKEN_SEMI,

  TOKEN_COMMA,
  TOKEN_PERIOD,

  TOKEN_BAND,
  TOKEN_BOR,
  TOKEN_BXOR,
  TOKEN_BNOT,

  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_QUE,

  TOKEN_LTHANEQ,
  TOKEN_GTHANEQ,
  TOKEN_NOTEQ,

  TOKEN_ADD,
  TOKEN_SUB,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_MOD,
  TOKEN_SHL,
  TOKEN_SHR,

  TOKEN_EQ,
  TOKEN_ADDEQ,
  TOKEN_SUBEQ,
  TOKEN_MULEQ,
  TOKEN_DIVEQ,
  TOKEN_MODEQ,
  TOKEN_BANDEQ,
  TOKEN_BOREQ,
  TOKEN_BXOREQ,
  TOKEN_SHLEQ,
  TOKEN_SHREQ,

  TOKEN_EQEQ,

  TOKEN_AT,
  TOKEN_HASH,
};

struct Token
{
  size_t line;
  size_t end;
  size_t len;
  TokenType type;
  std::string value;

  Token (size_t line, size_t end, size_t len, TokenType type,
         const std::string &value = "")
      : line{ line }, end{ end }, len{ len }, type{ type }, value{ value }
  {
  }

  std::string to_string () const;
};

class Lexer
{
public:
  Lexer () = default;

  std::vector<Token> tokenize (const std::string &src);
  std::vector<std::string> get_errors (const std::vector<Token> &toks) const;

private:
  std::string _src{};
  size_t _line{};
  size_t _llp{};
  size_t _pos{};

  Token process_id ();
  Token process_num ();
  Token process_numhex ();
  Token process_numbin ();
  Token process_numoct ();
  Token process_string ();
  Token process_symbol ();
  Token process_err ();

  void skip_comments ();

  bool is_start_of_id (char c) const;
  bool is_char_of_id (char c) const;

  Token gen_token (size_t len, size_t end, TokenType type,
                   const std::string &value = "") const;

  char peek (size_t pos) const;

  char get_spec_char (char c);

  std::string get_error (const Token &tok) const;

  std::string get_line (size_t linenum) const;

  bool is_processable (char c) const;
};

}

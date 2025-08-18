#pragma once

#include <cstddef>
#include <string>

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
  Token () = default;

  std::string to_string () const;
};

}

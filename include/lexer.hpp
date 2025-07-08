#pragma once

#include <map>
#include <string>
#include <vector>
namespace nlc
{

enum TokenType
{
  TOKEN_TYPE_UNK = 0,

  TOKEN_TYPE_ID,
  TOKEN_TYPE_EQ,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_FLOATING,
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_SYMBOL,
  TOKEN_TYPE_LPAREN,
  TOKEN_TYPE_RPAREN,
  TOKEN_TYPE_LBRACK,
  TOKEN_TYPE_RBRACK,
  TOKEN_TYPE_LBRACE,
  TOKEN_TYPE_RBRACE,
  TOKEN_TYPE_LARROW,
  TOKEN_TYPE_RARROW,
  TOKEN_TYPE_ASTERISK,
  TOKEN_TYPE_SLASH,
  TOKEN_TYPE_PLUS,
  TOKEN_TYPE_MINUS,
  TOKEN_TYPE_CACCENT,
  TOKEN_TYPE_AMPERSAND,
  TOKEN_TYPE_PIPE,
  TOKEN_TYPE_TILDE,
  TOKEN_TYPE_HASH,
  TOKEN_TYPE_PERCENT,
  TOKEN_TYPE_EXCLMARK,
  TOKEN_TYPE_COLON,
  TOKEN_TYPE_SEMICOL,
  TOKEN_TYPE_COMMA,
  TOKEN_TYPE_PERIOD,

  TOKEN_TYPE_PLUS_EQ,
  TOKEN_TYPE_MINUS_EQ,
  TOKEN_TYPE_ASTERISK_EQ,
  TOKEN_TYPE_SLASH_EQ,
  TOKEN_TYPE_PERCENT_EQ,
  TOKEN_TYPE_CACCENT_EQ,
  TOKEN_TYPE_PIPE_EQ,
  TOKEN_TYPE_AMPERSAND_EQ,
  TOKEN_TYPE_TILDE_EQ,
  TOKEN_TYPE_LARROW_EQ,
  TOKEN_TYPE_RARROW_EQ,
  TOKEN_TYPE_EXCLMARK_EQ,

  TOKEN_TYPE_EQ_EQ,
  TOKEN_TYPE_PLUS_PLUS,
  TOKEN_TYPE_MINUS_MINUS,
  TOKEN_TYPE_PIPE_PIPE,
  TOKEN_TYPE_AMPERSAND_AMPERSAND,
};

struct Token
{
public:
  TokenType type = TokenType::TOKEN_TYPE_UNK;
  std::string value{};

  Token () noexcept = default;
  Token (TokenType type, const std::string &value = "")
      : type (type), value (value)
  {
  }

  std::string
  to_string () const
  {
    std::string out = "Token {";

    out += " type: ";
    out += std::to_string (type);

    out += ", value: \"";
    out += value;
    out += "\"";

    out += " }";

    return out;
  }
};

class Lexer
{
public:
  Lexer () = default;

  std::vector<Token> tokenize (const std::string &src);

private:
  std::string src;
  size_t pos;
  size_t srclen;

  Token process_identifier ();
  Token process_number ();
  Token process_hexadecimal ();
  Token process_binary ();
  Token process_octal ();
  Token process_string ();
  Token process_symbol ();

  void skip_c_comments ();
  void skip_cpp_comments ();

  bool is_identifier_start (char c) const;
  bool is_identifier_char (char c) const;

  char get_special_char (char c) const;
};

}

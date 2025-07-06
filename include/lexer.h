#pragma once

#include <string>
#include <vector>

namespace nlc
{

enum TokenType
{
  TOKEN_TYPE_INVALID = 0,

  TOKEN_TYPE_ID,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_NUMBER_FLOATING,
  TOKEN_TYPE_EQ,
  TOKEN_TYPE_SYMBOL,
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_SEMICOL,
  TOKEN_TYPE_COLON,
  TOKEN_TYPE_COMMA,
  TOKEN_TYPE_PERIOD,
  TOKEN_TYPE_RPAREN,
  TOKEN_TYPE_LPAREN,
  TOKEN_TYPE_RBRACE,
  TOKEN_TYPE_LBRACE,
  TOKEN_TYPE_RBRACK,
  TOKEN_TYPE_LBRACK,
  TOKEN_TYPE_RARROW,
  TOKEN_TYPE_LARROW,
  TOKEN_TYPE_AMPERSAND,
  TOKEN_TYPE_ASTERISK,
  TOKEN_TYPE_PIPE,
  TOKEN_TYPE_CACCENT,
  TOKEN_TYPE_PLUS,
  TOKEN_TYPE_MINUS,
  TOKEN_TYPE_SLASH,
  TOKEN_TYPE_BSLASH,
  TOKEN_TYPE_EXCLMARK,
};

struct Token
{
  TokenType type = TokenType::TOKEN_TYPE_INVALID;

  std::string identifier{};
  long long ivalue{};
  double fvalue{};
  std::string strvalue{};
  char cvalue{};

  std::string
  to_string ()
  {
    std::string out{};

    out += "Token { ";

    out += "type: \"";
    out += tokentype_to_string (type);
    out += "\"";

    auto get_control_code = [] (char c) -> std::string {
      switch (c)
        {
        case '\n':
          return "<LINE FEED>";

        case '\t':
          return "<TAB>";

        case '\r':
          return "<CARRIAGE RETURN>";

        case '\0':
          return "<NULL>";

        default:
          std::string _outstr{};
          _outstr += c;
          return _outstr;
        }
    };

    auto format_strvalue = [&] (const std::string &strvalue) {
      std::string _out{};

      for (auto c : strvalue)
        {
          _out += get_control_code (c);
        }

      return _out;
    };

    switch (type)
      {
      case TokenType::TOKEN_TYPE_ID:
        {
          out += ", identifier: \"";
          out += identifier;
          out += "\"";
        }
        break;

      case TokenType::TOKEN_TYPE_NUMBER:
        {
          out += ", ivalue: ";
          out += std::to_string (ivalue);
        }
        break;

      case TokenType::TOKEN_TYPE_NUMBER_FLOATING:
        {
          out += ", fvalue: ";
          out += std::to_string (fvalue);
        }
        break;

      case TokenType::TOKEN_TYPE_STRING:
        {
          out += ", strvalue: \"";
          out += format_strvalue (strvalue);
          out += "\"";
        }
        break;

      case TokenType::TOKEN_TYPE_SYMBOL:
        {
          out += ", cvalue: '";
          out += get_control_code (cvalue);
          out += "'";
        }
        break;

      default:
        break;
      }

    out += " }";

    return out;
  }

private:
  constexpr const char *
  tokentype_to_string (TokenType type)
  {
    switch (type)
      {
      case TokenType::TOKEN_TYPE_ID:
        return "TOKEN_TYPE_ID";
      case TokenType::TOKEN_TYPE_NUMBER:
        return "TOKEN_TYPE_NUMBER";
      case TokenType::TOKEN_TYPE_NUMBER_FLOATING:
        return "TOKEN_TYPE_NUMBER_FLOATING";
      case TokenType::TOKEN_TYPE_EQ:
        return "TOKEN_TYPE_EQ";
      case TokenType::TOKEN_TYPE_SYMBOL:
        return "TOKEN_TYPE_SYMBOL";
      case TokenType::TOKEN_TYPE_STRING:
        return "TOKEN_TYPE_STRING";
      case TokenType::TOKEN_TYPE_SEMICOL:
        return "TOKEN_TYPE_SEMICOL";
      case TokenType::TOKEN_TYPE_COLON:
        return "TOKEN_TYPE_COLON";
      case TokenType::TOKEN_TYPE_COMMA:
        return "TOKEN_TYPE_COMMA";
      case TokenType::TOKEN_TYPE_PERIOD:
        return "TOKEN_TYPE_PERIOD";
      case TokenType::TOKEN_TYPE_RPAREN:
        return "TOKEN_TYPE_RPAREN";
      case TokenType::TOKEN_TYPE_LPAREN:
        return "TOKEN_TYPE_LPAREN";
      case TokenType::TOKEN_TYPE_RBRACE:
        return "TOKEN_TYPE_RBRACE";
      case TokenType::TOKEN_TYPE_LBRACE:
        return "TOKEN_TYPE_LBRACE";
      case TokenType::TOKEN_TYPE_RBRACK:
        return "TOKEN_TYPE_RBRACK";
      case TokenType::TOKEN_TYPE_LBRACK:
        return "TOKEN_TYPE_LBRACK";
      case TokenType::TOKEN_TYPE_RARROW:
        return "TOKEN_TYPE_RARROW";
      case TokenType::TOKEN_TYPE_LARROW:
        return "TOKEN_TYPE_LARROW";
      case TokenType::TOKEN_TYPE_AMPERSAND:
        return "TOKEN_TYPE_AMPERSAND";
      case TokenType::TOKEN_TYPE_ASTERISK:
        return "TOKEN_TYPE_ASTERISK";
      case TokenType::TOKEN_TYPE_PIPE:
        return "TOKEN_TYPE_PIPE";
      case TokenType::TOKEN_TYPE_CACCENT:
        return "TOKEN_TYPE_CACCENT";
      case TokenType::TOKEN_TYPE_PLUS:
        return "TOKEN_TYPE_PLUS";
      case TokenType::TOKEN_TYPE_MINUS:
        return "TOKEN_TYPE_MINUS";
      case TokenType::TOKEN_TYPE_SLASH:
        return "TOKEN_TYPE_SLASH";
      case TokenType::TOKEN_TYPE_BSLASH:
        return "TOKEN_TYPE_BLASH";
      case TokenType::TOKEN_TYPE_EXCLMARK:
        return "TOKEN_TYPE_EXCLMARK";
      default:
        return "";
      }
  }
};

class Lexer
{
public:
  Lexer () = default;

  std::vector<Token> get_tokens (const std::string &src);

private:
  size_t pos{};

  Token process_number (const std::string &src);
  Token process_hexadecimal (const std::string &src);
  Token process_binary (const std::string &src);
  Token process_octal (const std::string &src);
  Token process_decimal (const std::string &value);
  Token process_floating (const std::string &value);
  Token process_identifier (const std::string &src);
  Token process_string (const std::string &src);
  Token process_symbol (const std::string &src);

  char get_escape_character (const std::string &src);

  bool is_identifier_start (char c) const;
  bool is_identifier_char (char c) const;

  void skip_cpp_comments (const std::string &src);
  void skip_c_comments (const std::string &src);
};

}

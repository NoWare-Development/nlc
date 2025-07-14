#include "lexer.hpp"
#include <string>

namespace nlc
{

std::string tokentype_to_string (TokenType type);

std::string
Token::to_string () const
{
  std::string out = "Token {";

  out += " type: ";
  auto tokentype_str = tokentype_to_string (type);
  out += tokentype_str;
  out += ',';
  size_t padding = 19 - tokentype_str.length (); // 19 is length of
                                                 // the longest token type name
  for (size_t i = 0; i < padding; i++)
    {
      out += ' ';
    }

  out += "value: \"";
  out += value;
  out += "\"";

  out += " }";

  return out;
}

std::string
tokentype_to_string (TokenType type)
{
  switch (type)
    {
    case TokenType::TOKEN_TYPE_UNK:
      return "UNK";
    case TokenType::TOKEN_TYPE_ID:
      return "ID";
    case TokenType::TOKEN_TYPE_EQ:
      return "EQ";
    case TokenType::TOKEN_TYPE_NUMBER:
      return "NUMBER";
    case TokenType::TOKEN_TYPE_NUMBER_HEX:
      return "NUMBER_HEX";
    case TokenType::TOKEN_TYPE_NUMBER_BIN:
      return "NUMBER_BIN";
    case TokenType::TOKEN_TYPE_NUMBER_OCT:
      return "NUMBER_OCT";
    case TokenType::TOKEN_TYPE_FLOATING:
      return "FLOATING";
    case TokenType::TOKEN_TYPE_STRING:
      return "STRING";
    case TokenType::TOKEN_TYPE_SYMBOL:
      return "SYMBOL";
    case TokenType::TOKEN_TYPE_LPAREN:
      return "LPAREN";
    case TokenType::TOKEN_TYPE_RPAREN:
      return "RPAREN";
    case TokenType::TOKEN_TYPE_LBRACK:
      return "LBRACK";
    case TokenType::TOKEN_TYPE_RBRACK:
      return "RBRACK";
    case TokenType::TOKEN_TYPE_LBRACE:
      return "LBRACE";
    case TokenType::TOKEN_TYPE_RBRACE:
      return "RBRACE";
    case TokenType::TOKEN_TYPE_LARROW:
      return "LARROW";
    case TokenType::TOKEN_TYPE_RARROW:
      return "RARROW";
    case TokenType::TOKEN_TYPE_ASTERISK:
      return "ASTERISK";
    case TokenType::TOKEN_TYPE_SLASH:
      return "SLASH";
    case TokenType::TOKEN_TYPE_PLUS:
      return "PLUS";
    case TokenType::TOKEN_TYPE_MINUS:
      return "MINUS";
    case TokenType::TOKEN_TYPE_CACCENT:
      return "CACCENT";
    case TokenType::TOKEN_TYPE_AMPERSAND:
      return "AMPERSAND";
    case TokenType::TOKEN_TYPE_PIPE:
      return "PIPE";
    case TokenType::TOKEN_TYPE_TILDE:
      return "TILDE";
    case TokenType::TOKEN_TYPE_HASH:
      return "HASH";
    case TokenType::TOKEN_TYPE_PERCENT:
      return "PERCENT";
    case TokenType::TOKEN_TYPE_EXCLMARK:
      return "EXCLMARK";
    case TokenType::TOKEN_TYPE_COLON:
      return "COLON";
    case TokenType::TOKEN_TYPE_SEMICOL:
      return "SEMICOL";
    case TokenType::TOKEN_TYPE_COMMA:
      return "COMMA";
    case TokenType::TOKEN_TYPE_PERIOD:
      return "PERIOD";
    case TokenType::TOKEN_TYPE_QUEMARK:
      return "QUEMARK";
    case TokenType::TOKEN_TYPE_SHL:
      return "SHL";
    case TokenType::TOKEN_TYPE_SHR:
      return "SHR";
    case TokenType::TOKEN_TYPE_AT:
      return "AT";
    case TokenType::TOKEN_TYPE_PLUS_EQ:
      return "PLUS_EQ";
    case TokenType::TOKEN_TYPE_MINUS_EQ:
      return "MINUS_EQ";
    case TokenType::TOKEN_TYPE_ASTERISK_EQ:
      return "ASTERISK_EQ";
    case TokenType::TOKEN_TYPE_SLASH_EQ:
      return "SLASH_EQ";
    case TokenType::TOKEN_TYPE_PERCENT_EQ:
      return "PERCENT_EQ";
    case TokenType::TOKEN_TYPE_PIPE_EQ:
      return "PIPE_EQ";
    case TokenType::TOKEN_TYPE_CACCENT_EQ:
      return "CACCENT_EQ";
    case TokenType::TOKEN_TYPE_AMPERSAND_EQ:
      return "AMPERSAND_EQ";
    case TokenType::TOKEN_TYPE_TILDE_EQ:
      return "TILDE_EQ";
    case TokenType::TOKEN_TYPE_LARROW_EQ:
      return "LARROW_EQ";
    case TokenType::TOKEN_TYPE_RARROW_EQ:
      return "RARROW_EQ";
    case TokenType::TOKEN_TYPE_EXCLMARK_EQ:
      return "EXCLMARK_EQ";
    case TokenType::TOKEN_TYPE_SHL_EQ:
      return "SHL_EQ";
    case TokenType::TOKEN_TYPE_SHR_EQ:
      return "SHR_EQ";
    case TokenType::TOKEN_TYPE_EQ_EQ:
      return "EQ_EQ";
    case TokenType::TOKEN_TYPE_PLUS_PLUS:
      return "PLUS_PLUS";
    case TokenType::TOKEN_TYPE_MINUS_MINUS:
      return "MINUS_MINUS";
    case TokenType::TOKEN_TYPE_PIPE_PIPE:
      return "PIPE_PIPE";
    case TokenType::TOKEN_TYPE_AMPERSAND_AMPERSAND:
      return "AMPERSAND_AMPERSAND";
    case TokenType::TOKEN_TYPE_COLON_COLON:
      return "COLON_COLON";
    case TokenType::TOKEN_TYPE_LLONGARROW:
      return "LLONGARROW";
    case TokenType::TOKEN_TYPE_RLONGARROW:
      return "RLONGARROW";
    default:
      return "<Unidentified token type>";
    }
}

}

#include "lexer.hpp"

namespace nlc
{

constexpr const char *tokentype_to_string (TokenType type);

std::string
Token::to_string () const
{
  std::string out = "Token { ";

  out += "type: " + std::string (tokentype_to_string (type));

  bool print_value = !value.empty ();
  switch (type)
    {
    case TokenType::TOKEN_ID:
    case TokenType::TOKEN_NUM:
    case TokenType::TOKEN_NUMHEX:
    case TokenType::TOKEN_NUMBIN:
    case TokenType::TOKEN_NUMOCT:
    case TokenType::TOKEN_NUMFLOAT:
    case TokenType::TOKEN_STRING:
    case TokenType::TOKEN_SYMBOL:
      print_value = true;
      break;
    default:
      break;
    }

  if (print_value)
    {
      out += ", value: \"" + value + "\"";
    }

  out += " }";

  return out;
}

constexpr const char *
tokentype_to_string (TokenType type)
{
  switch (type)
    {
    case TokenType::TOKEN_ERR:
      return "ERR";

    case TokenType::TOKEN_ID:
      return "ID";

    case TokenType::TOKEN_NUM:
      return "NUM";
    case TokenType::TOKEN_NUMHEX:
      return "NUMHEX";
    case TokenType::TOKEN_NUMBIN:
      return "NUMBIN";
    case TokenType::TOKEN_NUMOCT:
      return "NUMOCT";
    case TokenType::TOKEN_NUMFLOAT:
      return "NUMFLOAT";

    case TokenType::TOKEN_STRING:
      return "STRING";
    case TokenType::TOKEN_SYMBOL:
      return "SYMBOL";

    case TokenType::TOKEN_LPAREN:
      return "LPAREN";
    case TokenType::TOKEN_RPAREN:
      return "RPAREN";
    case TokenType::TOKEN_LBRACK:
      return "LBRACK";
    case TokenType::TOKEN_RBRACK:
      return "RBRACK";
    case TokenType::TOKEN_LBRACE:
      return "LBRACE";
    case TokenType::TOKEN_RBRACE:
      return "RBRACE";

    case TokenType::TOKEN_LARROW:
      return "LARROW";
    case TokenType::TOKEN_RARROW:
      return "RARROW";

    case TokenType::TOKEN_LTHAN:
      return "LTHAN";
    case TokenType::TOKEN_GTHAN:
      return "GTHAN";

    case TokenType::TOKEN_COLON:
      return "COLON";
    case TokenType::TOKEN_DCOLON:
      return "DCOLON";

    case TokenType::TOKEN_SEMI:
      return "SEMI";

    case TokenType::TOKEN_COMMA:
      return "COMMA";
    case TokenType::TOKEN_PERIOD:
      return "PERIOD";

    case TokenType::TOKEN_BAND:
      return "BAND";
    case TokenType::TOKEN_BOR:
      return "BOR";
    case TokenType::TOKEN_BXOR:
      return "BXOR";
    case TokenType::TOKEN_BNOT:
      return "BNOT";

    case TokenType::TOKEN_AND:
      return "AND";
    case TokenType::TOKEN_OR:
      return "OR";
    case TokenType::TOKEN_NOT:
      return "NOT";
    case TokenType::TOKEN_QUE:
      return "QUE";

    case TokenType::TOKEN_LTHANEQ:
      return "LTHANEQ";
    case TokenType::TOKEN_GTHANEQ:
      return "GTHANEQ";
    case TokenType::TOKEN_NOTEQ:
      return "NOTEQ";

    case TokenType::TOKEN_ADD:
      return "ADD";
    case TokenType::TOKEN_SUB:
      return "SUB";
    case TokenType::TOKEN_MUL:
      return "MUL";
    case TokenType::TOKEN_DIV:
      return "DIV";
    case TokenType::TOKEN_MOD:
      return "MOD";
    case TokenType::TOKEN_SHL:
      return "SHL";
    case TokenType::TOKEN_SHR:
      return "SHR";

    case TokenType::TOKEN_EQ:
      return "EQ";
    case TokenType::TOKEN_ADDEQ:
      return "ADDEQ";
    case TokenType::TOKEN_SUBEQ:
      return "SUBEQ";
    case TokenType::TOKEN_MULEQ:
      return "MULEQ";
    case TokenType::TOKEN_DIVEQ:
      return "DIVEQ";
    case TokenType::TOKEN_MODEQ:
      return "MODEQ";
    case TokenType::TOKEN_BANDEQ:
      return "BANDEQ";
    case TokenType::TOKEN_BOREQ:
      return "BOREQ";
    case TokenType::TOKEN_BXOREQ:
      return "BXOREQ";
    case TokenType::TOKEN_SHLEQ:
      return "SHLEQ";
    case TokenType::TOKEN_SHREQ:
      return "SHREQ";

    case TokenType::TOKEN_EQEQ:
      return "EQEQ";

    case TokenType::TOKEN_AT:
      return "AT";
    case TokenType::TOKEN_HASH:
      return "HASH";

    default:
      return "<Unknown token type>";
    }
}

}

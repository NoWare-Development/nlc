#include "parser/parser.hpp"
#include "parser/ast.hpp"
#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>

namespace nlc
{

AST
Parser::parse ()
{
  AST prog (_pos, ASTType::AST_PROG);

  while (_pos < _tokens.size ())
    {
      auto top = parse_top ();
      if (_errored)
        {
          _errored = false;
          continue;
        }
      prog.append (top);
    }

  return prog;
}

AST
Parser::number_to_operand (Token &tok) const
{
  uint64_t value{};

  switch (tok.type)
    {
    case TokenType::TOKEN_NUM:
      value = string_to_dec (tok.value);
      break;

    case TokenType::TOKEN_NUMHEX:
      value = string_to_hex (tok.value);
      break;

    case TokenType::TOKEN_NUMBIN:
      value = string_to_bin (tok.value);
      break;

    case TokenType::TOKEN_NUMOCT:
      value = string_to_oct (tok.value);
      break;

    default:
      return {};
    }

  return AST (_pos, ASTType::AST_EXPR_OPERAND_NUM, hex_to_string (value));
}

uint64_t
Parser::string_to_dec (const std::string &str) const
{
  uint64_t out = 0;
  for (size_t i = 0; i < str.length (); i++)
    {
      out *= 10;
      out += str.at (i) - '0';
    }
  return out;
}

uint64_t
Parser::string_to_hex (const std::string &str) const
{
  uint64_t out = 0;
  for (size_t i = 0; i < str.length (); i++)
    {
      out <<= 4;
      char c = str.at (i);
      if (c >= 'a' && c <= 'f')
        {
          c = c - 'a' + 0xA;
        }
      else if (c >= 'A' && c <= 'F')
        {
          c = c - 'A' + 0xA;
        }
      else
        {
          c -= '0';
        }
      out |= c;
    }
  return out;
}

uint64_t
Parser::string_to_bin (const std::string &str) const
{
  uint64_t out = 0;
  for (size_t i = 0; i < str.length (); i++)
    {
      out <<= 1;
      out |= str.at (i) - '0';
    }
  return out;
}

uint64_t
Parser::string_to_oct (const std::string &str) const
{
  uint64_t out = 0;
  for (size_t i = 0; i < str.length (); i++)
    {
      out <<= 2;
      out += str.at (i) - '0';
    }
  return out;
}

std::string
Parser::hex_to_string (uint64_t val) const
{
  std::string out{};
  if (val > 0)
    {
      while (val > 0)
        {
          uint64_t rem = val % 16;
          char c;
          if (rem < 10)
            {
              c = rem + '0';
            }
          else
            {
              c = (rem - 10) + 'A';
            }
          out += c;
          val /= 16;
        }
      for (size_t i = 0, j = out.length () - 1; i < j; i++, j--)
        {
          std::swap (out[i], out[j]);
        }
    }
  else
    {
      out = '0';
    }
  return out;
}

void
Parser::skip_until (TokenType type)
{
  while (_pos < _tokens.size ())
    {
      if (_tokens.at (_pos).type == type)
        {
          break;
        }
      _pos++;
    }
}

bool
Parser::verify_pos (size_t pos)
{
  if (pos >= _tokens.size ())
    {
      add_error (ParserError::ErrType::PARSER_ERROR_EXPECTED, pos);
      return false;
    }

  return true;
}

bool
Parser::verify_token (size_t pos, TokenType got, TokenType expected)
{
  if (got != expected)
    {
      add_error (ParserError::ErrType::PARSER_ERROR_UNEXPECTED, pos);
      return false;
    }
  return true;
}

bool
Parser::verify_value (size_t pos, const std::string &got,
                      const std::string &expected)
{
  if (got != expected)
    {
      add_error (ParserError::ErrType::PARSER_ERROR_EXPECTED, pos);
      return false;
    }
  return true;
}

TokenType
Parser::peek (size_t pos) const
{
  if (pos >= _tokens.size ())
    {
      return TokenType::TOKEN_ERR;
    }
  return _tokens.at (pos).type;
}

bool
Parser::is_modifier (const std::string &str) const
{
  for (const auto &mod : _modifiers)
    {
      if (str == mod)
        {
          return true;
        }
    }
  return false;
}

bool
Parser::is_operator (TokenType type) const
{
  return is_binary_operator (type) || is_assign_operator (type)
         || is_compare_operator (type) || is_boolean_operator (type)
         || is_prefix_operator (type);
}

#define IS_IN_MAP(map, val) (map).find ((val)) != (map).end ()

bool
Parser::is_binary_operator (TokenType type) const
{
  return IS_IN_MAP (_binary_operators, type);
}

bool
Parser::is_assign_operator (TokenType type) const
{
  return IS_IN_MAP (_assign_operators, type);
}

bool
Parser::is_compare_operator (TokenType type) const
{
  return IS_IN_MAP (_compare_operators, type);
}

bool
Parser::is_boolean_operator (TokenType type) const
{
  return IS_IN_MAP (_boolean_operators, type);
}

bool
Parser::is_prefix_operator (TokenType type) const
{
  return IS_IN_MAP (_prefix_operators, type);
}

bool
Parser::is_operator (ASTType type) const
{
  return is_binary_operator (type) || is_assign_operator (type)
         || is_compare_operator (type) || is_boolean_operator (type)
         || is_prefix_operator (type);
}

bool
Parser::is_binary_operator (ASTType type) const
{
  return ((type >> 8) & 0xFF) == AST_GROUP_EXPR_BINARY_OPERATOR;
}

bool
Parser::is_assign_operator (ASTType type) const
{
  return ((type >> 8) & 0xFF) == AST_GROUP_EXPR_ASSIGN_OPERATOR;
}

bool
Parser::is_compare_operator (ASTType type) const
{
  return ((type >> 8) & 0xFF) == AST_GROUP_EXPR_COMPARE_OPERATOR;
}

bool
Parser::is_boolean_operator (ASTType type) const
{
  return ((type >> 8) & 0xFF) == AST_GROUP_EXPR_BOOLEAN_OPERATOR;
}

bool
Parser::is_prefix_operator (ASTType type) const
{
  return ((type >> 8) & 0xFF) == AST_GROUP_EXPR_PREFIX_OPERATOR;
}

bool
Parser::is_operand (ASTType type) const
{
  return ((type & 0xFF00) >> 8) == AST_GROUP_EXPR_OPERAND || type == AST_EXPR
         || type == AST_FROM_MODULE;
}

bool
Parser::is_numeric_token (TokenType type) const
{
  for (const auto &t : _numeric_tokens)
    {
      if (t == type)
        return true;
    }
  return false;
}

}

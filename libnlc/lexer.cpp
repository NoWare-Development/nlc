#include "lexer.h"

#include <cctype>
#include <cmath>
#include <vector>

std::vector<Token>
Lexer::get_tokens (const std::string &src)
{
  pos = 0;

  const size_t srclen = src.length ();
  std::vector<Token> tokens{};

  while (pos < srclen)
    {
      char c = src.at (pos);
      if (isspace (c))
        {
          pos++;
          continue;
        }

      if (isdigit (c))
        {
          tokens.push_back (process_number (src));
          continue;
        }

      if (is_identifier_start (c))
        {
          tokens.push_back (process_identifier (src));
          continue;
        }
      if (c == '"')
        {
          tokens.push_back (process_string (src));
          continue;
        }

      if (c == '\'')
        {
          tokens.push_back (process_symbol (src));
          continue;
        }

      switch (c)
        {
        case '=':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_EQ });
          }
          break;

        case ';':
          {
            tokens.push_back (
                (Token){ .type = TokenType::TOKEN_TYPE_SEMICOL });
          }
          break;

        case ':':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_COLON });
          }
          break;

        case ',':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_COMMA });
          }
          break;

        case '.':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_PERIOD });
          }
          break;

        case '(':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_LPAREN });
          }
          break;
        case ')':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_RPAREN });
          }
          break;

        case '{':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_LBRACE });
          }
          break;
        case '}':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_RBRACE });
          }
          break;

        case '[':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_LBRACK });
          }
          break;
        case ']':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_RBRACK });
          }
          break;

        case '<':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_LARROW });
          }
          break;
        case '>':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_RARROW });
          }
          break;

        case '&':
          {
            tokens.push_back (
                (Token){ .type = TokenType::TOKEN_TYPE_AMPERSAND });
          }
          break;
        case '*':
          {
            tokens.push_back (
                (Token){ .type = TokenType::TOKEN_TYPE_ASTERISK });
          }
          break;
        case '^':
          {
            tokens.push_back (
                (Token){ .type = TokenType::TOKEN_TYPE_CACCENT });
          }
          break;
        case '|':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_PIPE });
          }
          break;
        case '+':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_PLUS });
          }
          break;
        case '-':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_MINUS });
          }
          break;
        case '/':
          {
            if (src.at (pos + 1) == '/')
              {
                // Skip C++ styled comments
                skip_cpp_comments (src);
                break;
              }
            else if (src.at (pos + 1) == '*')
              {
                // Skip C styled comments
                skip_c_comments (src);
                break;
              }
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_SLASH });
          }
          break;
        case '\\':
          {
            tokens.push_back ((Token){ .type = TokenType::TOKEN_TYPE_BSLASH });
          }
          break;

        case '!':
          {
            tokens.push_back (
                (Token){ .type = TokenType::TOKEN_TYPE_EXCLMARK });
          }
          break;

        default:
          // TODO: error
          break;
        }

      pos++;
    }

  return tokens;
}

Token
Lexer::process_number (const std::string &src)
{
  char c = src.at (pos);
  if (c == '0')
    {
      char c2 = src.at (pos + 1);

      if (isdigit (c2))
        {
          return process_octal (src);
        }

      switch (c2)
        {
        case 'x':
          return process_hexadecimal (src);

        case 'b':
          return process_binary (src);

        case '.':
          return {};

        default:
          Token zero{};
          zero.type = TokenType::TOKEN_TYPE_NUMBER;
          zero.ivalue = 0;
          pos++;
          return zero;
        }
    }

  std::string numbuf{};
  while (pos < src.length ())
    {
      c = src.at (pos);
      if (c == '\'' || c == '_')
        {
          pos++;
          continue;
        }

      if (!isdigit (c) && c != '.')
        {
          break;
        }

      numbuf += c;
      pos++;
    }

  if (numbuf.find ('.') != numbuf.npos)
    {
      return process_floating (numbuf);
    }

  return process_decimal (numbuf);
}

Token
Lexer::process_hexadecimal (const std::string &src)
{
  Token out{};
  out.type = TokenType::TOKEN_TYPE_NUMBER;

  pos += 2;
  while (pos < src.length ())
    {
      char c = src.at (pos);

      if (!isxdigit (c))
        {
          break;
        }

      if (c >= 'A')
        {
          c = c - 'A' + 0xA;
        }
      else if (c >= 'a')
        {
          c = c - 'a' + 0xA;
        }
      else
        {
          c -= '0';
        }

      out.ivalue = (out.ivalue << 4) | c;
      pos++;
    }

  return out;
}

Token
Lexer::process_binary (const std::string &src)
{
  Token out{};
  out.type = TokenType::TOKEN_TYPE_NUMBER;

  pos += 2;
  while (pos < src.length ())
    {
      char c = src.at (pos);
      if (c != '1' && c != '0')
        {
          break;
        }

      out.ivalue = (out.ivalue << 1) | (c & 1);
      pos++;
    }

  return out;
}

Token
Lexer::process_octal (const std::string &src)
{ // TODO: implementation.
  return {};
}

Token
Lexer::process_decimal (const std::string &value)
{
  Token out{};
  out.type = TokenType::TOKEN_TYPE_NUMBER;

  for (auto c : value)
    {
      out.ivalue = (out.ivalue * 10) + c - '0';
    }

  return out;
}

Token
Lexer::process_floating (const std::string &value)
{
  Token out{};
  out.type = TokenType::TOKEN_TYPE_NUMBER_FLOATING;

  bool after_dot = false;

  double before_dot_part{};
  double after_dot_part{};
  size_t after_dot_times = 1;

  for (char c : value)
    {
      if (c == '.')
        {
          if (after_dot)
            {
              return {};
            }

          after_dot = true;
          continue;
        }

      c -= '0';

      if (after_dot)
        {
          after_dot_part += (double)c / pow (10, after_dot_times++);
        }
      else
        {
          before_dot_part = (before_dot_part * 10) + c;
        }
    }

  out.fvalue = after_dot_part + before_dot_part;

  return out;
}

Token
Lexer::process_identifier (const std::string &src)
{
  Token out{};
  out.type = TokenType::TOKEN_TYPE_ID;
  out.identifier = src.at (pos++);

  while (pos < src.length ())
    {
      char c = src.at (pos);
      if (!is_identifier_char (c))
        {
          break;
        }
      out.identifier += c;
      pos++;
    }

  return out;
}

Token
Lexer::process_string (const std::string &src)
{
  Token out{};
  out.type = TokenType::TOKEN_TYPE_STRING;

  pos++;
  while (pos < src.length ())
    {
      char c = src.at (pos++);
      if (c == '"')
        break;

      if (c == '\\')
        {
          c = get_escape_character (src);
        }

      out.strvalue += c;
    }

  if (out.strvalue.length () == 0)
    {
      out.strvalue += '\0';
    }

  return out;
}

Token
Lexer::process_symbol (const std::string &src)
{
  Token out{};
  out.type = TokenType::TOKEN_TYPE_SYMBOL;

  pos++;
  std::string buf{};
  while (pos < src.length ())
    {
      char c = src.at (pos++);
      if (c == '\'')
        break;

      if (c == '\\')
        {
          c = get_escape_character (src);
        }

      buf += c;
    }

  if (buf.length () > 1)
    {
      // TODO: error
      return {};
    }
  if (buf.length () == 1)
    {
      out.cvalue = buf.at (0);
    }

  return out;
}

char
Lexer::get_escape_character (const std::string &src)
{
  if (pos >= src.length ())
    {
      return -1;
    }

  char c = src.at (pos++);
  switch (c)
    {
    case 'n':
      return '\n';
    case 't':
      return '\t';
    case 'r':
      return '\r';
    case '\\':
      return '\\';
    case '\"':
      return '\"';
    case '0':
      return '\0';
    case '\'':
      return '\'';
    }

  return -1;
}

bool
Lexer::is_identifier_start (char c) const
{
  return isalpha (c) || c == '$' || c == '_';
}

bool
Lexer::is_identifier_char (char c) const
{
  return is_identifier_start (c) || isdigit (c);
}

void
Lexer::skip_cpp_comments (const std::string &src)
{
  pos += 2;

  while (pos < src.length ())
    {
      char c = src.at (pos++);
      if (c == '\n')
        {
          break;
        }
    }
}

void
Lexer::skip_c_comments (const std::string &src)
{
  pos += 2;

  while (pos < src.length ())
    {
      char c = src.at (pos++);
      if (c == '*')
        {
          // Comment end
          if (src.at (pos) == '/')
            {
              pos++;
              break;
            }
        }
    }
}

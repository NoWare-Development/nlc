#include "lexer.hpp"
#include <cctype>
#include <iostream>
#include <vector>

namespace nlc
{

std::vector<Token>
Lexer::tokenize (const std::string &src)
{
  this->src = src;
  pos = 0;
  srclen = src.length ();

  std::vector<Token> out{};

  while (pos < srclen)
    {
      char c = src.at (pos);
      if (isspace (c))
        {
          pos++;
          continue;
        }

      if (is_identifier_start (c))
        {
          out.push_back (process_identifier ());
          continue;
        }

      if (isdigit (c))
        {
          out.push_back (process_number ());
          continue;
        }

      if (c == '/')
        {
          if (pos + 1 < srclen)
            {
              char next = src.at (pos + 1);
              if (next == '/')
                {
                  pos += 2;
                  skip_cpp_comments ();
                  continue;
                }
              else if (next == '*')
                {
                  pos += 2;
                  skip_c_comments ();
                  continue;
                }
            }
        }

      if (c == '"')
        {
          out.push_back (process_string ());
          continue;
        }
      else if (c == '\'')
        {
          out.push_back (process_symbol ());
          continue;
        }

      switch (c)
        {
        case '=':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_EQ_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_EQ);
          }
          break;

        case '(':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_LPAREN);
          }
          break;
        case ')':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_RPAREN);
          }
          break;

        case '[':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_LBRACK);
          }
          break;
        case ']':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_RBRACK);
          }
          break;

        case '{':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_LBRACE);
          }
          break;
        case '}':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_RBRACE);
          }
          break;

        case '<':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_LARROW_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_LARROW);
          }
          break;
        case '>':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_RARROW_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_RARROW);
          }
          break;

        case '*':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_ASTERISK_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_ASTERISK);
          }
          break;

        case '/':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_SLASH_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_SLASH);
          }
          break;

        case '+':
          {
            if (pos + 1 < srclen)
              {
                char c2 = src.at (pos + 1);
                if (c2 == '+')
                  {
                    out.emplace_back (TokenType::TOKEN_TYPE_PLUS_PLUS);
                    pos++;
                    break;
                  }
                else if (c2 == '=')
                  {
                    out.emplace_back (TokenType::TOKEN_TYPE_PLUS_EQ);
                    pos++;
                    break;
                  }
              }
            out.emplace_back (TokenType::TOKEN_TYPE_PLUS);
          }
          break;
        case '-':
          {
            if (pos + 1 < srclen)
              {
                char c2 = src.at (pos + 1);
                if (c2 == '-')
                  {
                    out.emplace_back (TokenType::TOKEN_TYPE_MINUS_MINUS);
                    pos++;
                    break;
                  }
                else if (c2 == '=')
                  {
                    out.emplace_back (TokenType::TOKEN_TYPE_MINUS_EQ);
                    pos++;
                    break;
                  }
              }
            out.emplace_back (TokenType::TOKEN_TYPE_MINUS);
          }
          break;

        case '^':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_CACCENT_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_CACCENT);
          }
          break;

        case '&':
          {
            if (pos + 1 < srclen)
              {
                char c2 = src.at (pos + 1);
                if (c2 == '&')
                  {
                    out.emplace_back (
                        TokenType::TOKEN_TYPE_AMPERSAND_AMPERSAND);
                    pos++;
                    break;
                  }
                else if (c2 == '=')
                  {
                    out.emplace_back (TokenType::TOKEN_TYPE_AMPERSAND_EQ);
                    pos++;
                    break;
                  }
              }
            out.emplace_back (TokenType::TOKEN_TYPE_AMPERSAND);
          }
          break;

        case '|':
          {
            if (pos + 1 < srclen)
              {
                char c2 = src.at (pos + 1);
                if (c2 == '|')
                  {
                    out.emplace_back (TokenType::TOKEN_TYPE_PIPE_PIPE);
                    pos++;
                    break;
                  }
                else if (c2 == '=')
                  {
                    out.emplace_back (TokenType::TOKEN_TYPE_PIPE_EQ);
                    pos++;
                    break;
                  }
              }
            out.emplace_back (TokenType::TOKEN_TYPE_PIPE);
          }
          break;

        case '~':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_TILDE_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_TILDE);
          }
          break;

        case '#':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_HASH);
          }
          break;

        case '%':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_PERCENT_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_PERCENT);
          }
          break;

        case '!':
          {
            if (pos + 1 < srclen && src.at (pos + 1) == '=')
              {
                out.emplace_back (TokenType::TOKEN_TYPE_EXCLMARK_EQ);
                pos++;
                break;
              }
            out.emplace_back (TokenType::TOKEN_TYPE_EXCLMARK);
          }
          break;

        case ':':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_COLON);
          }
          break;
        case ';':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_SEMICOL);
          }
          break;
        case ',':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_COMMA);
          }
          break;
        case '.':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_PERIOD);
          }
          break;

        case '?':
          {
            out.emplace_back (TokenType::TOKEN_TYPE_QUEMARK);
          }
          break;
        }

      pos++;
    }

  return out;
}

Token
Lexer::process_identifier ()
{
  std::string name{};
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (!is_identifier_char (c))
        {
          break;
        }

      name += c;
      pos++;
    }

  return Token (TokenType::TOKEN_TYPE_ID, name);
}

Token
Lexer::process_number ()
{
  char c = src.at (pos);
  if (c == '0')
    {
      char next = src.at (pos + 1);
      if (isdigit (next))
        {
          return process_octal ();
        }

      switch (next)
        {
        case 'x':
          return process_hexadecimal ();
        case 'b':
          return process_binary ();
        default:
          pos++;
          return Token (TokenType::TOKEN_TYPE_NUMBER, "0");
        }
    }

  std::string buf{};
  bool floating = false;
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (!isdigit (c) && c != '.')
        {
          if (c == '\'' || c == '_')
            {
              pos++;
              continue;
            }

          break;
        }

      if (c == '.')
        {
          if (!floating)
            {
              floating = true;
            }
          else
            {
              // TODO: error double dot in number.
              return Token ();
            }
        }

      buf += c;
      pos++;
    }

  return Token (floating ? TokenType::TOKEN_TYPE_FLOATING
                         : TokenType::TOKEN_TYPE_NUMBER,
                buf);
}

Token
Lexer::process_hexadecimal ()
{
  std::string val = "0x";

  pos += 2;
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (!isxdigit (c))
        {
          break;
        }

      val += c;
      pos++;
    }

  return Token (TokenType::TOKEN_TYPE_NUMBER, val);
}

Token
Lexer::process_binary ()
{
  std::string val = "0b";

  pos += 2;
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (c != '1' && c != '0')
        {
          break;
        }

      val += c;
      pos++;
    }

  return Token (TokenType::TOKEN_TYPE_NUMBER, val);
}

Token
Lexer::process_octal ()
{
  std::string val = "0";

  pos += 1;
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (c < '0' || c > '7')
        {
          break;
        }

      val += c;
      pos++;
    }

  return Token (TokenType::TOKEN_TYPE_NUMBER, val);
}

Token
Lexer::process_string ()
{
  std::string str{};

  pos++;
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (c == '"')
        {
          pos++;
          break;
        }

      if (c == '\\')
        {
          char c2 = src.at (pos + 1);
          str += get_special_char (c2);
          pos += 2;
          continue;
        }

      str += c;
      pos++;
    }

  return Token (TokenType::TOKEN_TYPE_STRING, str);
}

Token
Lexer::process_symbol ()
{
  std::string sym{};

  pos++;
  char c = src.at (pos++);
  if (c == '\\')
    {
      char c2 = src.at (pos++);
      sym += get_special_char (c2);
    }
  else
    {
      sym += c;
    }

  if (src.at (pos) != '\'')
    {
      // TODO: error -- unterminated character OR character is too
      // long.
      return Token ();
    }
  pos++;

  return Token (TokenType::TOKEN_TYPE_SYMBOL, sym);
}

void
Lexer::skip_c_comments ()
{
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (c == '*')
        {
          if (pos + 1 < srclen && src.at (pos + 1) == '/')
            {
              pos += 2;
              break;
            }
        }
      pos++;
    }
}

void
Lexer::skip_cpp_comments ()
{
  while (pos < srclen)
    {
      char c = src.at (pos);
      if (c == '\n')
        {
          break;
        }
      pos++;
    }
}

bool
Lexer::is_identifier_start (char c) const
{
  return isalpha (c) || c == '_' || c == '$';
}

bool
Lexer::is_identifier_char (char c) const
{
  return is_identifier_start (c) || isdigit (c);
}

char
Lexer::get_special_char (char c) const
{
  switch (c)
    {
    case '0':
      return 0;
    case 'n':
      return '\n';
    case 't':
      return '\t';
    case 'r':
      return '\r';
    case '\'':
      return '\'';
    case '"':
      return '"';
    case '\\':
      return '\\';

    default:
      // TODO: error -- unknown special character.
      return 0;
    }
}
}

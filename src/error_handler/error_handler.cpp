#include "error_handler.hpp"
#include "lexer.hpp"
#include "util.hpp"
#include <iostream>
#include <string>

constexpr const char *error_or_errors[2] = { "error", "errors" };

void
ErrorHandler::add_tokens (const std::vector<nlc::Token> &tokens)
{
  _tokens = tokens;
}

void
ErrorHandler::add_parser_errors (
    const std::vector<nlc::Parser::ParserError> &errors)
{
  _parser_errors = errors;
}

bool
ErrorHandler::handle_tokens () const
{
  if (has_invalid_tokens ())
    {
      print_errored_tokens ();
      return false;
    }
  return true;
}

bool
ErrorHandler::handle_parser_errors () const
{
  if (has_parser_errors ())
    {
      print_parser_errors ();
      return false;
    }

  return true;
}

void
ErrorHandler::print_errored_tokens () const
{
  auto get_invalid_tokens
      = [] (const std::vector<nlc::Token> &tokens) -> std::vector<nlc::Token> {
    std::vector<nlc::Token> invalid_tokens{};

    for (auto &tok : tokens)
      {
        if (tok.type == nlc::TokenType::TOKEN_ERR)
          {
            invalid_tokens.push_back (tok);
          }
      }
    return invalid_tokens;
  };

  std::vector<nlc::Token> invalid_tokens = get_invalid_tokens (_tokens);

  std::cout << std::to_string (invalid_tokens.size ()) << " "
            << error_or_errors[invalid_tokens.size () > 1]
            << " has occured during \"" << _filename << "\" compilation:\n";
  for (auto &tok : invalid_tokens)
    {
      std::cout << get_token_error (tok) << '\n';
    }
}

void
ErrorHandler::print_parser_errors () const
{
  std::cout << std::to_string (_parser_errors.size ()) << " "
            << error_or_errors[_parser_errors.size () > 1]
            << " has occured during \"" << _filename << "\" compilation:\n";
  for (auto &err : _parser_errors)
    {
      std::cout << get_parser_error (err) << '\n';
    }
}

std::string
ErrorHandler::get_token_error (const nlc::Token &tok) const
{
  std::string error_string{};
  error_string += "Failed to process token \"";
  error_string += tok.value;
  error_string += "\" ";
  error_string += "at (" + std::to_string (tok.line + 1) + ":"
                  + std::to_string (tok.end - (tok.len - 1)) + ")";
  error_string += '\n';

  error_string += get_highlighted_token (tok);

  return error_string;
}

std::string
ErrorHandler::get_parser_error (const nlc::Parser::ParserError &err) const
{
  std::string error_string{};

  auto &tok = _tokens.at (err.pos);
  error_string += "Failed to parse: " + get_parser_error_reason (err) + " at ("
                  + std::to_string (tok.line + 1) + ":"
                  + std::to_string (tok.end - (tok.len - 1)) + ")";
  error_string += '\n';

  error_string += get_highlighted_token (tok);

  return error_string;
}

std::string
ErrorHandler::get_highlighted_token (const nlc::Token &tok) const
{
  std::string out{};

  std::string linenum = std::to_string (tok.line + 1) + ": ";
  std::string line = get_line (tok.line);

  out += linenum + line + '\n';

  size_t start = tok.end - tok.len + linenum.length () + 1;
  for (size_t i = 0; i < start; i++)
    {
      out += ' ';
    }
  out += nlc::escape_color (nlc::ESCColor::ESCCOLOR_RED);
  out += nlc::escape_graphics (nlc::ESCGraphics::ESCGRAPHICS_BOLD);
  out += '^';

  for (size_t i = 0; i < tok.len - 1; i++)
    {
      out += '~';
    }
  out += nlc::escape_reset ();
  return out;
}

std::string
ErrorHandler::get_parser_error_reason (
    const nlc::Parser::ParserError &err) const
{
  using namespace nlc;

  switch (err.type)
    {
    case Parser::ParserErrorType::PARSER_ERROR_UNK:
      return "<Unknown error>";

    case Parser::ParserErrorType::PARSER_ERROR_EXPECTED:
      {
        std::string out{};
        out += "Expected token \"" + err.msg + "\"";
        return out;
      }

    case Parser::ParserErrorType::PARSER_ERROR_UNEXPECTED:
      {
        std::string out{};
        out += "Unexpected token \"" + err.msg + "\"";
        return out;
      }

    default:
      return {};
    }
}

bool
ErrorHandler::has_invalid_tokens () const
{
  for (auto &tok : _tokens)
    {
      if (tok.type == nlc::TokenType::TOKEN_ERR)
        {
          return true;
        }
    }
  return false;
}

bool
ErrorHandler::has_parser_errors () const
{
  return _parser_errors.size () > 0;
}

std::string
ErrorHandler::get_line (size_t linenum) const
{
  size_t pos = 0;
  // Skip until found line by number
  while (pos < _src.length () && linenum > 0)
    {
      if (_src.at (pos++) == '\n')
        {
          linenum--;
        }
    }

  std::string buf{};
  while (pos < _src.length ())
    {
      char c = _src.at (pos++);
      if (c == '\n')
        {
          break;
        }
      buf += c;
    }
  return buf;
}

#pragma once

#include "parser.hpp"
#include <lexer.hpp>
#include <vector>

class ErrorHandler
{
public:
  ErrorHandler (const std::string &filename, const std::string &src)
      : _filename (filename), _src (src)
  {
  }

  void add_tokens (const std::vector<nlc::Token> &tokens);
  void add_parser_errors (const std::vector<nlc::Parser::ParserError> &errors);

  bool handle_tokens () const;
  bool handle_parser_errors () const;

private:
  std::vector<nlc::Token> _tokens{};
  std::vector<nlc::Parser::ParserError> _parser_errors{};

  std::string _filename;
  std::string _src;

  void print_errored_tokens () const;
  void print_parser_errors () const;

  std::string get_token_error (const nlc::Token &tok) const;
  std::string get_parser_error (const nlc::Parser::ParserError &err) const;

  std::string get_highlighted_token (const nlc::Token &tok) const;

  std::string
  get_parser_error_reason (const nlc::Parser::ParserError &err) const;

  bool has_invalid_tokens () const;
  bool has_parser_errors () const;

  std::string get_line (size_t linenum) const;
};

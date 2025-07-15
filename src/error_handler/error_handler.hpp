#pragma once

#include <lexer.hpp>
#include <vector>

class ErrorHandler
{
public:
  ErrorHandler () = default;

  bool handle_errored_tokens (nlc::Lexer &lexer, const std::string &file,
                              const std::vector<nlc::Token> &tokens) const;
};

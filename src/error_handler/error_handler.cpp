#include "error_handler.hpp"
#include "util.hpp"

bool
ErrorHandler::handle_errored_tokens (
    nlc::Lexer &lexer, const std::string &file,
    const std::vector<nlc::Token> &tokens) const
{
  auto lexer_errors = lexer.get_errors (tokens);
  if (lexer_errors.size () > 0)
    {
      static const char *error_or_errors[2] = {
        "error",
        "errors",
      };
      std::cerr << nlc::escape_color (nlc::ESCColor::ESCCOLOR_RED)
                << nlc::escape_graphics (nlc::ESCGraphics::ESCGRAPHICS_BOLD)
                << lexer_errors.size () << " "
                << error_or_errors[lexer_errors.size () > 1]
                << nlc::escape_reset () << " in file " << file << ":\n";
      for (auto &e : lexer_errors)
        {
          std::cerr << e << '\n';
        }
      std::cerr << '\n';
      return false;
    }
  return true;
}

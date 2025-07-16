#include "error_handler/error_handler.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <lexer.hpp>

int
main (int argc, char **argv)
{
  if (argc < 2)
    {
      std::cout << "Usage: nlc <file>\n";
      return 0;
    }

  std::ifstream file (argv[1]);
  if (!file.is_open ())
    {
      std::cout << "Failed to open file \"" << argv[1] << "\"\n";
      return -1;
    }
  char buf[1024 * 40] = { 0 };
  file.read (buf, 1024 * 40);
  const std::string src (buf);

  ErrorHandler handler (argv[1], src);

  nlc::Lexer lexer{};
  auto tokens = lexer.tokenize (src);
  for (auto &tok : tokens)
    {
      std::cout << tok.to_string () << '\n';
    }
  handler.add_tokens (tokens);
  if (!handler.handle_tokens ())
    {
      return -2;
    }

  nlc::Parser parser (tokens);
  auto csts = parser.parse ();
  auto errors = parser.get_errors ();
  handler.add_parser_errors (errors);
  if (!handler.handle_parser_errors ())
    {
      return -3;
    }
  std::cout << '\n';

  return 0;
}

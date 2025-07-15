#include "error_handler/error_handler.hpp"
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

  ErrorHandler handler{};

  nlc::Lexer lexer{};
  auto tokens = lexer.tokenize (src);
  // for (auto &tok : tokens)
  //   {
  //     std::cout << tok.to_string () << '\n';
  //   }

  if (!handler.handle_errored_tokens (lexer, argv[1], tokens))
    {
      return -2;
    }

  return 0;
}

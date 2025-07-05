#include "args.h"
#include <fstream>
#include <iostream>
#include <lexer.h>

void
print_help ()
{
  // TODO: implementation
}

int
main (int argc, char **argv)
{
  ArgParser arg_parser (argc, argv);
  if (!arg_parser.has_arguments ())
    {
      print_help ();
      return 0;
    }

  auto files = arg_parser.get_dangling_arguments ();
  std::string file{};
  if (files.size () > 0)
    {
      file = files[0];
    }
  else
    {
      std::cerr << "You must provide source files.\n";
      return -1;
    }

  std::fstream file_stream = std::fstream (file);
  if (!file_stream.is_open ())
    {
      std::cerr << "Failed to open file \"" << file << "\"\n";
      return -2;
    }
  char src[8192] = {};
  file_stream.read (src, 8192);
  file_stream.close ();

  Lexer lexer{};
  auto tokens = lexer.get_tokens (src);

  for (auto &tok : tokens)
    {
      if (tok.type == TokenType::TOKEN_TYPE_INVALID)
        {
          std::cerr << "Found invalid token\n";
          return -3;
        }
      std::cout << tok.to_string () << '\n';
    }
  std::cout << "Total number of tokens: " << std::to_string (tokens.size ())
            << '\n';

  return 0;
}

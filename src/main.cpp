#include <cstddef>
#include <fstream>
#include <iostream>
#include <lexer.hpp>
#include <parser.hpp>

int
main (int argc, char **argv)
{
  // TODO: proper args parser
  if (argc < 2)
    {
      return -1;
    }

  std::fstream file (argv[1]);
  if (!file.is_open ())
    {
      return -2;
    }

  constexpr size_t max_file_size = 8192;
  char buf[max_file_size] = { 0 };
  file.read (buf, 8192);
  file.close ();
  std::string src (buf);

  nlc::Lexer lexer{};
  auto tokens = lexer.tokenize (src);
  for (auto &tok : tokens)
    {
      std::cout << tok.to_string () << '\n';
    }

  nlc::Parser parser{};
  auto cst = parser.parse (tokens);
  std::cout << cst.to_string (0) << '\n';

  return 0;
}

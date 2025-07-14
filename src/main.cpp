#include <cstddef>
#include <fstream>
#include <iostream>
#include <lexer.hpp>
#include <parser.hpp>
#include <vector>

void
print_tokens (const std::vector<nlc::Token> &tokens)
{
  const size_t tokens_len = tokens.size ();
  auto get_number_len = [] (size_t num) -> size_t {
    size_t len = 0;
    while (num > 0)
      {
        len++;
        num /= 10;
      }
    return len;
  };

  size_t max_padding = get_number_len (tokens_len) - 1;

  for (size_t i = 0; i < tokens_len; i++)
    {
      size_t current_padding = max_padding - (get_number_len (i) - 1);
      if (i == 0)
        {
          current_padding = max_padding;
        }

      for (size_t j = 0; j < current_padding; j++)
        {
          std::cout << ' ';
        }
      std::cout << '[' << i << "] " << tokens.at (i).to_string () << '\n';
    }
  std::cout << '\n';
}

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
  // for (size_t i = 0; i < tokens.size (); i++)
  //   {
  //     std::cout << '[' << i << "] " << tokens.at (i).to_string () << '\n';
  //   }
  // std::cout << '\n';
  print_tokens (tokens);

  nlc::Parser parser{};
  auto cst = parser.parse (tokens);
  std::cout << cst.to_string () << '\n';

  return 0;
}

#include "argparser.hpp"
#include "config/config.hpp"
#include "error_handler/error_handler.hpp"
#include "info/info.hpp"
#include "util/util.hpp"
#include <fstream>
#include <iostream>
#include <lexer/lexer.hpp>
#include <map>
#include <parser/parser.hpp>
#include <sstream>
#include <unordered_map>

int
main (int argc, char **argv)
{
  // if (argc < 2)
  //   {
  //     std::cout << "Usage: nlc <file>\n";
  //     return 0;
  //   }
  //
  // std::ifstream file (argv[1]);
  // if (!file.is_open ())
  //   {
  //     std::cout << "Failed to open file \"" << argv[1] << "\"\n";
  //     return -1;
  //   }
  // char buf[1024 * 40] = { 0 };
  // file.read (buf, 1024 * 40);
  // const std::string src (buf);
  //
  // ErrorHandler handler (argv[1], src);
  //
  // nlc::Lexer lexer{};
  // auto tokens = lexer.tokenize (src);
  // for (size_t i = 0; i < tokens.size (); i++)
  //   {
  //     std::cout << "(" << i << ") " << tokens.at (i).to_string () << '\n';
  //   }
  // handler.add_tokens (tokens);
  // if (!handler.handle_tokens ())
  //   {
  //     return -2;
  //   }
  //
  // nlc::Parser parser (tokens);
  // auto asts = parser.parse ();
  // auto errors = parser.get_errors ();
  // handler.add_parser_errors (errors);
  // std::cout << asts.to_string ();
  // std::cout << '\n';
  // if (!handler.handle_parser_errors ())
  //   {
  //     return -3;
  //   }
  // if (!handler.handle_invalid_expressions (asts))
  //   {
  //     return -4;
  //   }
  ArgParser argparser (argc, argv);
  if (argparser.has_argument ("help"))
    {
      info::display_help ();
      return 0;
    }
  else if (argparser.has_argument ("version"))
    {
      info::display_version ();
      return 0;
    }

  auto optlevel = argparser.get_argument_value<size_t> ("opt");
  nlc::Config::get_instance ()->set_optimization_level (optlevel);

  auto includes = argparser.get_argument_value_list<std::string> ("include");
  nlc::Config::get_instance ()->set_include_paths (includes);

  auto libpaths = argparser.get_argument_value_list<std::string> ("libpath");
  nlc::Config::get_instance ()->set_link_lib_search_paths (libpaths);

  auto libs = argparser.get_argument_value_list<std::string> ("linkwith");
  nlc::Config::get_instance ()->set_link_libs (libs);

  auto has_nostdlib = argparser.has_argument ("nostdlib");
  if (has_nostdlib)
    {
      nlc::Config::get_instance ()->set_compiler_flag (
          nlc::Config::CompilerFlag::COMPILER_FLAG_NOSTDLIB);
    }

  auto has_freestanding = argparser.has_argument ("freestanding");
  if (has_freestanding)
    {
      nlc::Config::get_instance ()->set_compiler_flag (
          nlc::Config::CompilerFlag::COMPILER_FLAG_FREESTANDING);
    }

  auto sources = argparser.get_dangling_arguments ();
  if (sources.empty ())
    {
      std::cout << escape_graphics (ESCGraphics::ESCGRAPHICS_BOLD)
                << "nlc: " << escape_color (ESCColor::ESCCOLOR_RED)
                << "fatal error: " << escape_reset ()
                << "no input files\ncompilation terminated.\n";
    }

  for (const auto &srcfile : sources)
    {
      std::ifstream file (srcfile);
      if (!file.is_open ())
        {
          std::cout << escape_graphics (ESCGraphics::ESCGRAPHICS_BOLD)
                    << "nlc: " << escape_color (ESCColor::ESCCOLOR_RED)
                    << "fatal error: " << escape_reset ()
                    << "failed to open file \"" << srcfile << "\"\n";
          return -1;
        }
      std::stringstream buffer;
      buffer << file.rdbuf ();
      std::string src = buffer.str ();

      ErrorHandler handler (srcfile, src);

      nlc::Lexer lexer{};
      auto tokens = lexer.tokenize (src);

      // TODO: remove me
      for (size_t i = 0; i < tokens.size (); i++)
        {
          std::cout << "(" << i << ") " << tokens.at (i).to_string () << '\n';
        }
      handler.add_tokens (tokens);
      if (!handler.handle_tokens ())
        {
          return -2;
        }

      nlc::Parser parser (tokens);
      auto root = parser.parse ();
      auto errors = parser.get_errors ();
      handler.add_parser_errors (errors);

      // TODO: remove me
      std::cout << root.to_string () << '\n';
      if (!handler.handle_parser_errors ())
        {
          return -3;
        }
      if (!handler.handle_invalid_expressions (root))
        {
          return -4;
        }
    }

  return 0;
}

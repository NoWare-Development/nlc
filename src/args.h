#pragma once

#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

class ArgParser
{
public:
  ArgParser (int argc, char **argv)
  {
    if (argc - 1 == 0)
      return;

    arguments.reserve (argc - 1);

    for (int i = 0; i < argc - 1; i++)
      {
        arguments.push_back (argv[i + 1]);
      }
  }

  template <class T>
  T
  get_argument_value (const std::string &str_full,
                      const std::string &str_small = "") const
  {
    for (auto &arg : arguments)
      {
        std::string found_string{};
        std::string_view view{ arg };
        bool is_small = false;

        if (view.at (0) != '-')
          continue;

        do
          {
            if (view.find (str_full) != view.npos)
              {
                found_string = str_full;
                break;
              }

            if (!str_small.empty () && view.find (str_small) != view.npos)
              {
                found_string = str_small;
                is_small = true;
              }
          }
        while (0);

        if (found_string.empty ())
          continue;

        std::string value_str{};
        if (is_small)
          {
            value_str = std::string (view.substr (found_string.length ()));
          }
        else
          {
            if (view.substr (found_string.length ()).at (0) != '=')
              {
                std::cerr << "(0) Failed to parse argument \"" << arg
                          << "\"\n";
                return {};
              }
            value_str = std::string (view.substr (found_string.length () + 1));
          }

        if (value_str.empty ())
          {
            std::cerr << "(1) Failed to parse argument \"" << arg << "\"\n";
            return {};
          }

        return parse_arg<T> (value_str);
      }

    return {};
  }

  bool
  has_argument (const std::string &str) const
  {
    for (auto &arg : arguments)
      {
        if (arg == str)
          return true;
      }
    return false;
  }

  std::vector<std::string>
  get_dangling_arguments () const
  {
    std::vector<std::string> out{};

    for (auto &arg : arguments)
      {
        if (arg.at (0) != '-')
          out.push_back (arg);
      }

    return out;
  }

  bool
  has_arguments () const
  {
    return !arguments.empty ();
  }

private:
  std::vector<std::string> arguments{};

  template <class T, class U = std::__or_<std::is_integral<T>,
                                          std::is_same<T, std::string>>>
  T
  parse_arg (const std::string &str) const
  {
    if constexpr (std::is_same<std::string, T>::value)
      {
        return str;
      }
    else if constexpr (std::is_floating_point<T>::value)
      {
        return std::stof (str);
      }
    else if constexpr (std::is_same<bool, T>::value)
      {
        std::string lc_str;
        for (auto c : str)
          {
            lc_str.push_back (tolower (c));
          }
        return lc_str == "true" || lc_str == "1" ? true : false;
      }
    else if constexpr (std::is_integral<T>::value)
      {
        return std::stoi (str);
      }

    return {};
  }
};

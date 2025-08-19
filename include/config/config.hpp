#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace nlc
{

class Config
{
public:
  ~Config () = default;

  static Config *
  get_instance ()
  {
    static Config *_instance = nullptr;
    if (_instance == nullptr)
      {
        _instance = new Config ();
      }
    return _instance;
  }

  void set_include_paths (std::vector<std::string> paths);
  const std::vector<std::string> get_include_paths () const;

  void set_link_lib_search_paths (std::vector<std::string> paths);
  const std::vector<std::string> get_link_lib_search_paths () const;

  void set_link_libs (std::vector<std::string> libs);
  const std::vector<std::string> get_link_libs () const;

  void set_optimization_level (size_t level);
  size_t get_optimization_level () const;

  enum CompilerFlag
  {
    COMPILER_FLAG_NOSTDLIB = 1 << 0,
    COMPILER_FLAG_FREESTANDING = 1 << 1,
  };
  void set_compiler_flag (CompilerFlag flag);
  uint32_t get_compiler_flags () const;

  enum CompilerAction
  {
    COMPILER_ACTION_COMPILE_LINK,
    COMPILER_ACTION_COMPILE,
    COMPILER_ACTION_GET_IR,
    COMPILER_ACTION_GET_ASSEMBLY,
  };
  void set_compiler_action (CompilerAction act);
  CompilerAction get_compiler_action () const;

private:
  Config () = default;

  std::vector<std::string> _include_paths{};

  std::vector<std::string> _link_lib_search_paths{};
  std::vector<std::string> _link_libs{};

  size_t _optimization_level{};

  uint32_t _compiler_flags{};

  CompilerAction _compiler_action{};
};

}

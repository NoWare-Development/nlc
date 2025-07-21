#include "parser.hpp"

#define __CST_TYPE_GROUP_(type) (((type) >> 8) & 0xFF)
#define __CST_TYPE_INDEX_(type) ((type) & 0xFF)

namespace nlc
{

std::string csttype_to_string (CSTType type);

void
CST::append (CST child)
{
  child.change_depth (depth + 1);
  children.push_back (child);
}

void
CST::change_depth (size_t depth)
{
  this->depth = depth;
  for (auto &child : children)
    {
      child.change_depth (depth + 1);
    }
}

std::string
CST::to_string () const
{
  std::string out = "";
  for (size_t i = 0; i < depth; i++)
    {
      out += "  ";
    }
  if (depth > 0)
    {
      out += "(" + std::to_string (depth) + ") ";
    }

  out += "CST { ";

  out += "type: ";
  out += csttype_to_string (type);
  out += ", value: \"";
  out += value;
  out += "\"";

  out += " }";
  out += '\n';

  if (children.size () > 0)
    {
      for (auto &child : children)
        {
          out += child.to_string ();
        }
    }

  return out;
}

std::string
csttype_to_string (CSTType type)
{
  constexpr const char *prefixes[0x100] = {
    "",
    "DECL_",
    "STMT_",
    "MODULE_",
  };

  constexpr const char *groups[0x100][0x100] = {
    {
        // Groupless
        "PROG",
        "STMTLIST",
        "IMPORT",
        "MACRO",
        "EXPR",
    },
    {
        // DECL
        "VAR_DECL",
    },
    {
        // STMT
        "RETURN",
    },
    {
        // MODULE
        "NAME",
        "ITEM",
        "EVERYTHING",
    },
  };

  std::string out{};
  out += prefixes[__CST_TYPE_GROUP_ (type)];
  out += groups[__CST_TYPE_GROUP_ (type)][__CST_TYPE_INDEX_ (type)];
  return out;
}

}

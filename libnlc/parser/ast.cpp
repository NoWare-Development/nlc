#include "ast.hpp"

namespace nlc
{

static std::string asttype_to_string (ASTType type);

std::string
AST::to_string () const
{
  std::string out{};

  if (_depth > 0)
    {
      for (size_t i = 0; i < _depth; i++)
        {
          out += "  ";
        }

      out += "(" + std::to_string (_depth) + ") ";
    }

  out += "AST: " + asttype_to_string (type) + " \"" + value + "\"";

  if (!children.empty ())
    {
      for (auto &child : children)
        {
          out += '\n' + child.to_string ();
        }
    }

  return out;
}

bool
AST::is_empty () const
{
  return children.empty ();
}

void
AST::set_depth (size_t depth)
{
  _depth = depth;
  for (auto &child : children)
    {
      child.set_depth (depth + 1);
    }
}

AST *
AST::append (AST child)
{
  child.set_depth (_depth + 1);
  children.push_back (child);
  return &children.at (children.size () - 1);
}

#define __AST_GET_GROUP(type) (((type) & 0xFF00) >> 8)
#define __AST_GET_ID(type) ((type) & 0xFF)

static std::string
asttype_to_string (ASTType type)
{
  constexpr const char *groups[0x100] = {
    "",
    "STMT",
    "ENUM",
    "TYPE",
    "VAR",
    "FUNC",
    "EXPR_BINARY_OPERATOR",
    "EXPR_COMPARE_OPERATOR",
    "EXPR_ASSIGN_OPERATOR",
    "EXPR_PREFIX_OPERATOR",
    "EXPR_OPERAND",
    "TEMPLATE",
    "VISIBILITY_MARKER",
    "INITLIST_ENTRY",
  };

  constexpr const char *ids[0x100][0x100] = {
    {
        // Groupless
        "ERR",

        "PROG",
        "EXPR",
        "MODULE",
        "IMPORT",
        "TYPEDEF",
        "EXPORT",
        "MODIFIER",
        "NONE",
        "LABEL",
        "VARIADIC",
        "TEMPLATE",
        "STRUCT",
        "FROM_MODULE",
        "INITLIST",
    },
    {
        // STMT
        "LIST",
        "RETURN",
        "GOTO",
        "LABEL",
        "BREAK",
        "CONTINUE",
        "WHILE",
        "FOR",
        "DOWHILE",
        "EXPR",
        "SWITCH",
        "CASE",
        "DEFAULT",
        "DEFER",
        "IF",
        "ELSE",
    },
    {
        // ENUM
        "DEF",
        "ELEMENT",
    },
    {
        // TYPE
        "PLAIN",
        "ARRAY",
        "POINTER",
    },
    {
        // VAR
        "DECL",
        "DEF",
    },
    {
        // FUNC
        "PROTO",
        "DECL",
        "DEF",
        "ARGLIST",
    },
    {
        // EXPR_BINARY_OPERATOR
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "MOD",
        "SHL",
        "SHR",
        "AND",
        "OR",
        "XOR",
    },
    {
        // EXPR_COMPARE_OPERATOR
        "EQ",
        "NOTEQ",
        "LTHAN",
        "GTHAN",
        "LTHANEQ",
        "GTHANEQ",
    },
    {
        // EXPR_ASSIGN_OPERATOR
        "EQ",
        "ADDEQ",
        "SUBEQ",
        "MULEQ",
        "DIVEQ",
        "MODEQ",
        "SHLEQ",
        "SHREQ",
        "ANDEQ",
        "OREQ",
        "XOREQ",
    },
    {
        // EXPR_PREFIX_OPERATOR
        "NOT",
        "BNOT",
        "NEG",
        "DEREF",
        "ADDRESS",
    },
    {
        // EXPR_OPERAND
        "IDENTIFIER",
        "NUM",
        "NUMFLOAT",
        "ARRAY_ELEMENT",
        "CAST_TO",
        "CALL",
        "NUMTYPESPEC",
        "STRING",
        "SYMBOL",
        "ACCESS_MEMBER",
    },
    {
        // TEMPLATE
        "TYPE_LIST",
        "TYPE",
    },
    {
        // VISIBILITY_MARKER
        "PUBLIC",
        "PRIVATE",
    },
    {
        // INITLIST_ENTRY
        "INIT",
        "INIT_EXPLICIT",
    },
  };

  size_t group = __AST_GET_GROUP (type);
  size_t id = __AST_GET_ID (type);

  std::string out = groups[group];
  if (!out.empty ())
    {
      out += '_';
    }
  out += ids[group][id];

  return out;
}
}

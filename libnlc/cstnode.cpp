#include "parser.hpp"
#include <string>

namespace nlc
{

std::string cstnodetype_to_string (CSTNodeType type);

void
CSTNode::append (CSTNode child)
{
  child.change_depth (depth + 1);
  children.push_back (child);
}

std::string
CSTNode::to_string () const
{
  std::string out{};

  if (depth > 0)
    {
      for (size_t i = 0; i < depth; i++)
        {
          out += "  ";
        }
      out += "(" + std::to_string (depth) + ") ";
    }

  out += "CSTNode {";

  out += " type: ";
  out += cstnodetype_to_string (type);

  if (!value.empty ())
    {
      out += ", value: \"";
      out += value;
      out += "\"";
    }

  out += " }\n";

  for (auto &child : children)
    {
      out += child.to_string ();
    }

  return out;
}

void
CSTNode::change_depth (size_t depth)
{
  this->depth = depth;
  for (auto &child : children)
    {
      child.change_depth (depth + 1);
    }
}

std::string
cstnodetype_to_string (CSTNodeType type)
{
  constexpr const char *group_prefixes[] = {
    "",          "STMT_",  "IDSTMT_", "CASESTMT_", "OP_",     "PREFIXOP_",
    "ASSIGNOP_", "CMPOP_", "BOOLOP_", "PREOP_",    "POSTOP_", "OPERAND_",
  };

  constexpr const char *group_items[16][16] = {
    {
        // groupless
        "UNK",
        "PROGRAM",
        "STMTLIST",
        "EXPR",
    },
    {
        // STMT
        "IF",
        "ELSE",
        "SWITCH",
        "DEREF",
        "RETURN",
        "BREAK",
        "CONTINUE",
        "WHILE",
        "FOR",
    },
    {
        // IDSTMT
        "VAR_ASSIGN_TO_EXPR",
        "VAR_ASSIGN_TO_EXPRLIST",
        "ELEM_ASSIGN_TO_EXPR",
        "ELEM_ASSIGN_TO_EXPRLIST",
        "CALL",
        "LABEL",
    },
    {
        // CASESTMT
        "CASE",
        "DEFAULT",
    },
    {
        // OP
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "MOD",
        "AND",
        "OR",
        "XOR",
    },
    {
        // PREFIXOP
        "BOOLNOT",
        "MINUS",
        "NOT",
    },
    {
        // ASSIGNOP
        "EQUAL",
        "ADD_EQ",
        "SUB_EQ",
        "MUL_EQ",
        "DIV_EQ",
        "MOD_EQ",
        "AND_EQ",
        "OR_EQ",
        "XOR_EQ",
    },
    {
        // CMPOP
        "EQUAL",
        "NOT_EQUAL",
        "LESS",
        "GREATER",
        "LESS_OR_EQUAL",
        "GREATER_OR_EQUAL",
    },
    {
        // BOOLOP
        "AND",
        "OR",
    },
    {
        // PREOP
        "INCREMENT",
        "DECREMENT",
    },
    {
        // POSTOP
        "INCREMENT",
        "DECREMENT",
    },
    {
        // OPERAND
        "INTEGER",
        "FLOATING",
        "ID",
    },
  };

  std::string out = group_prefixes[type >> 4];
  out += group_items[type >> 4][type & 15];
  return out;
}

}

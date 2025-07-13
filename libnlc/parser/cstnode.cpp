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
  constexpr const char *group_prefixes[0x100] = {
    "",           "STMT_",     "IDSTMT_",       "CASESTMT_", "OP_",
    "PREFIXOP_",  "ASSIGNOP_", "CMPOP_",        "BOOLOP_",   "PREOP_",
    "POSTOP_",    "OPERAND_",  "IDCREATESTMT_", "IDCREATE_", "IDTYPE_",
    "IDTYPERAW_",
  };

  constexpr const char *group_items[0x100][0x100] = {
    {
        // groupless
        "UNK",
        "PROGRAM",
        "STMTLIST",
        "EXPR",
        "TYPEMODIFIER",
        "FUNCTION_PROTOTYPE",
    },
    {
        // STMT
        "IF",
        "ELSE",
        "SWITCH",
        "RETURN",
        "BREAK",
        "CONTINUE",
        "WHILE",
        "FOR",
        "GOTO",
        "LABEL",
    },
    {
        // IDSTMT
        "CREATE",
        "ASSIGN",
        "FUNC_DECLARATION",
        "FUNC_DEFINITION",
        "CALL",
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
    {
        // IDCREATESTMT
        "WITH_ASSIGN",
        "WITHOUT_ASSIGN",
    },
    {
        // IDCREATE
        "WITH_MOD",
        "PLAIN",
        "LIST",
    },
    {
        // IDTYPE
        "POINTER",
        "PLAIN",
    },
    {
        // IDTYPERAW
        "PLAIN",
        "ARRAY",
        "FUNCPTR",
    },
  };

  std::string out = group_prefixes[__NLC_CSTNODETYPE_GET_GROUP_ (type)];
  out += group_items[__NLC_CSTNODETYPE_GET_GROUP_ (type)]
                    [__NLC_CSTNODETYPE_GET_ID_ (type)];
  return out;
}

}

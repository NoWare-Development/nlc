#include "parser.hpp"
#include <string>

namespace nlc
{

std::string cstnodetype_to_string (CSTNodeType type);

std::string
CSTNode::to_string (size_t depth) const
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
      out += child.to_string (depth + 1);
    }

  return out;
}

std::string
cstnodetype_to_string (CSTNodeType type)
{
  switch (type)
    {
    case CSTNodeType::CST_NODE_TYPE_UNK:
      return "UNK";

    case CSTNodeType::CST_NODE_TYPE_PROGRAM:
      return "PROGRAM";

    case CSTNodeType::CST_NODE_TYPE_STMTLIST:
      return "STMTLIST";

    case CSTNodeType::CST_NODE_TYPE_STMT_IF:
      return "STMT_IF";

    case CSTNodeType::CST_NODE_TYPE_STMT_ELSE:
      return "STMT_ELSE";

    case CSTNodeType::CST_NODE_TYPE_STMT_SWITCH:
      return "STMT_SWITCH";

    case CSTNodeType::CST_NODE_TYPE_STMT_DEREF:
      return "STMT_DEREF";

    case CSTNodeType::CST_NODE_TYPE_STMT_RETURN:
      return "STMT_RETURN";

    case CSTNodeType::CST_NODE_TYPE_IDSTMT_VAR_ASSIGN_TO_EXPR:
      return "IDSTMT_VAR_ASSIGN_TO_EXPR";
    case CSTNodeType::CST_NODE_TYPE_IDSTMT_VAR_ASSIGN_TO_EXPRLIST:
      return "IDSTMT_VAR_ASSIGN_TO_EXPRLIST";
    case CSTNodeType::CST_NODE_TYPE_IDSTMT_ELEM_ASSIGN_TO_EXPR:
      return "IDSTMT_ELEM_ASSIGN_TO_EXPR";
    case CSTNodeType::CST_NODE_TYPE_IDSTMT_ELEM_ASSIGN_TO_EXPRLIST:
      return "IDSTMT_ELEM_ASSIGN_TO_EXPRLIST";
    case CSTNodeType::CST_NODE_TYPE_IDSTMT_CALL:
      return "IDSTMT_CALL";
    case CSTNodeType::CST_NODE_TYPE_IDSTMT_LABEL:
      return "IDSTMT_LABEL";

    case CSTNodeType::CST_NODE_TYPE_CASESTMT_CASE:
      return "CASESTMT_CASE";
    case CSTNodeType::CST_NODE_TYPE_CASESTMT_DEFAULT:
      return "CASESTMT_DEFAULT";

    case CSTNodeType::CST_NODE_TYPE_EXPR:
      return "EXPR";

    case CSTNodeType::CST_NODE_TYPE_OP_ADD:
      return "OP_ADD";
    case CSTNodeType::CST_NODE_TYPE_OP_SUB:
      return "OP_SUB";
    case CSTNodeType::CST_NODE_TYPE_OP_MUL:
      return "OP_MUL";
    case CSTNodeType::CST_NODE_TYPE_OP_DIV:
      return "OP_DIV";
    case CSTNodeType::CST_NODE_TYPE_OP_MOD:
      return "OP_MOD";
    case CSTNodeType::CST_NODE_TYPE_OP_AND:
      return "OP_AND";
    case CSTNodeType::CST_NODE_TYPE_OP_OR:
      return "OP_OR";
    case CSTNodeType::CST_NODE_TYPE_OP_XOR:
      return "OP_XOR";
    case CSTNodeType::CST_NODE_TYPE_OP_BOOLNOT:
      return "OP_BOOLNOT";
    case CSTNodeType::CST_NODE_TYPE_OP_NOT:
      return "OP_NOT";

    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_EQUAL:
      return "ASSIGNOP_EQUAL";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_ADD_EQ:
      return "ASSIGNOP_ADD_EQ";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_SUB_EQ:
      return "ASSIGNOP_SUB_EQ";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_MUL_EQ:
      return "ASSIGNOP_MUL_EQ";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_DIV_EQ:
      return "ASSIGNOP_DIV_EQ";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_MOD_EQ:
      return "ASSIGNOP_MOD_EQ";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_AND_EQ:
      return "ASSIGNOP_AND_EQ";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_OR_EQ:
      return "ASSIGNOP_OR_EQ";
    case CSTNodeType::CST_NODE_TYPE_ASSIGNOP_XOR_EQ:
      return "ASSIGNOP_XOR_EQ";

    case CSTNodeType::CST_NODE_TYPE_CMPOP_EQUAL:
      return "CMPOP_EQUAL";
    case CSTNodeType::CST_NODE_TYPE_CMPOP_NOT_EQUAL:
      return "CMPOP_NOT_EQUAL";
    case CSTNodeType::CST_NODE_TYPE_CMPOP_LESS:
      return "CMPOP_LESS";
    case CSTNodeType::CST_NODE_TYPE_CMPOP_GREATER:
      return "CMPOP_GREATER";
    case CSTNodeType::CST_NODE_TYPE_CMPOP_LESS_OR_EQUAL:
      return "CMPOP_LESS_OR_EQUAL";
    case CSTNodeType::CST_NODE_TYPE_CMPOP_GREATER_OR_EQUAL:
      return "CMPOP_GREATER_OR_EQUAL";

    case CSTNodeType::CST_NODE_TYPE_BOOLOP_AND:
      return "BOOLOP_AND";
    case CSTNodeType::CST_NODE_TYPE_BOOLOP_OR:
      return "BOOLOP_OR";

    default:
      return {};
    }
}

}

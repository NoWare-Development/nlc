#include <lexer.h>
#include <string>

int
hello_world_test ()
{
  std::string src = R"(
    // Import module "io" from "std"
    import std.io;

    /*
     * Print hello and return 0
     */
    int main(int argc, char **argv) {
      println("Hello, World!" // Strange position for comment
    /* And a second comment */
        );
      return 0;
    }
  )";

  Lexer lexer{};
  auto tokens = lexer.get_tokens (src);

  const std::vector<Token> expected_tokens = {
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "import" },
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "std" },
    Token{ .type = TokenType::TOKEN_TYPE_PERIOD },
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "io" },
    Token{ .type = TokenType::TOKEN_TYPE_SEMICOL },

    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "int" },
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "main" },
    Token{ .type = TokenType::TOKEN_TYPE_LPAREN },
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "int" },
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "argc" },
    Token{ .type = TokenType::TOKEN_TYPE_COMMA },
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "char" },
    Token{ .type = TokenType::TOKEN_TYPE_ASTERISK },
    Token{ .type = TokenType::TOKEN_TYPE_ASTERISK },
    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "argv" },
    Token{ .type = TokenType::TOKEN_TYPE_RPAREN },
    Token{ .type = TokenType::TOKEN_TYPE_LBRACE },

    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "println" },
    Token{ .type = TokenType::TOKEN_TYPE_LPAREN },
    Token{ .type = TokenType::TOKEN_TYPE_STRING, .strvalue = "Hello, World!" },
    Token{ .type = TokenType::TOKEN_TYPE_RPAREN },
    Token{ .type = TokenType::TOKEN_TYPE_SEMICOL },

    Token{ .type = TokenType::TOKEN_TYPE_ID, .identifier = "return" },
    Token{ .type = TokenType::TOKEN_TYPE_NUMBER, .ivalue = 0 },
    Token{ .type = TokenType::TOKEN_TYPE_SEMICOL },

    Token{ .type = TokenType::TOKEN_TYPE_RBRACE },
  };

  if (expected_tokens.size () != tokens.size ())
    {
      return -1;
    }

  for (size_t i = 0; i < tokens.size (); i++)
    {
      auto &tok = tokens.at (i);
      auto &exp = expected_tokens.at (i);

      if (tok.type != exp.type || tok.identifier != exp.identifier
          || tok.ivalue != exp.ivalue || tok.fvalue != exp.fvalue
          || tok.strvalue != exp.strvalue || tok.cvalue != exp.cvalue)
        {
          return -1;
        }
    }

  return 0;
}

int
main ()
{
  return hello_world_test ();
}

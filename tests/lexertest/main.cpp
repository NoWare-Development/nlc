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

  nlc::Lexer lexer{};
  auto tokens = lexer.get_tokens (src);

  const std::vector<nlc::Token> expected_tokens = {
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID,
                .identifier = "import" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "std" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_PERIOD },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "io" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_SEMICOL },

    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "int" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "main" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_LPAREN },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "int" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "argc" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_COMMA },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "char" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ASTERISK },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ASTERISK },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID, .identifier = "argv" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_RPAREN },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_LBRACE },

    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID,
                .identifier = "println" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_LPAREN },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_STRING,
                .strvalue = "Hello, World!" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_RPAREN },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_SEMICOL },

    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_ID,
                .identifier = "return" },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_NUMBER, .ivalue = 0 },
    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_SEMICOL },

    nlc::Token{ .type = nlc::TokenType::TOKEN_TYPE_RBRACE },
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

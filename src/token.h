#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  TOKEN_EOF,

  TOKEN_AT_APP,
  TOKEN_FUNC,
  TOKEN_RETURN,

  TOKEN_INT,

  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  TOKEN_ARROW,
  TOKEN_SEMICOLON,

  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_EQ
} TokenType;

typedef struct {
  TokenType type;
  char *lexeme;
} Token;

#endif

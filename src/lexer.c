#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// helpers (GLOBAL SCOPE)

static char current(Lexer *l) { return l->src[l->pos]; }
static void advance(Lexer *l) { l->pos++; }

void lexer_init(Lexer *lexer, const char *src) {
  lexer->src = src;
  lexer->pos = 0;
}

Token lexer_next(Lexer *l) {

  while (isspace(current(l)))
    advance(l);

  if (current(l) == '\0')
    return (Token){TOKEN_EOF, ""};

  // @app
  if (current(l) == '@') {
    advance(l);

    int start = l->pos;
    while (isalnum(current(l)))
      advance(l);

    char *word = strndup(l->src + start, l->pos - start);

    if (!strcmp(word, "app"))
      return (Token){TOKEN_AT_APP, word};

    return (Token){TOKEN_IDENTIFIER, word};
  }

  // keywords / identifiers
  if (isalpha(current(l))) {
    int start = l->pos;

    while (isalnum(current(l)))
      advance(l);

    char *word = strndup(l->src + start, l->pos - start);

    if (!strcmp(word, "func"))
      return (Token){TOKEN_FUNC, word};

    if (!strcmp(word, "return"))
      return (Token){TOKEN_RETURN, word};

    return (Token){TOKEN_IDENTIFIER, word};
  }

  // numbers
  if (isdigit(current(l))) {
    int start = l->pos;

    while (isdigit(current(l)))
      advance(l);

    return (Token){TOKEN_NUMBER, strndup(l->src + start, l->pos - start)};
  }

  // operators / symbols
  switch (current(l)) {

  case '-':
    if (l->src[l->pos + 1] == '>') {
      advance(l);
      advance(l);
      return (Token){TOKEN_ARROW, "->"};
    }
    advance(l);
    return (Token){TOKEN_MINUS, "-"};

  case '+':
    advance(l);
    return (Token){TOKEN_PLUS, "+"};

  case '*':
    advance(l);
    return (Token){TOKEN_STAR, "*"};

  case '/':
    advance(l);
    return (Token){TOKEN_SLASH, "/"};

  case '(':
    advance(l);
    return (Token){TOKEN_LPAREN, "("};

  case ')':
    advance(l);
    return (Token){TOKEN_RPAREN, ")"};

  case '{':
    advance(l);
    return (Token){TOKEN_LBRACE, "{"};

  case '}':
    advance(l);
    return (Token){TOKEN_RBRACE, "}"};

  case ';':
    advance(l);
    return (Token){TOKEN_SEMICOLON, ";"};
  }

  // unknown char → skip
  advance(l);
  return lexer_next(l);
}

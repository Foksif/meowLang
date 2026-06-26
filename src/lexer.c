#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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

  if (current(l) == '@') {
    advance(l);

    int start = l->pos;
    while (isalnum(current(l)))
      advance(l);

    char *word = strndup(l->src + start, l->pos - start);

    if (!strcmp(word, "app"))
      return (Token){TOKEN_AT_APP, word};
  }

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

  if (isdigit(current(l))) {
    int start = l->pos;

    while (isdigit(current(l)))
      advance(l);

    return (Token){TOKEN_NUMBER, strndup(l->src + start, l->pos - start)};
  }

  if (current(l) == '(') {
    advance(l);
    return (Token){TOKEN_LPAREN, "("};
  }

  if (current(l) == ')') {
    advance(l);
    return (Token){TOKEN_RPAREN, ")"};
  }

  if (current(l) == '{') {
    advance(l);
    return (Token){TOKEN_LBRACE, "{"};
  }

  if (current(l) == '}') {
    advance(l);
    return (Token){TOKEN_RBRACE, "}"};
  }

  if (current(l) == ';') {
    advance(l);
    return (Token){TOKEN_SEMICOLON, ";"};
  }

  if (current(l) == '-' && l->src[l->pos + 1] == '>') {
    advance(l);
    advance(l);
    return (Token){TOKEN_ARROW, "->"};
  }

  advance(l);
  return (Token){TOKEN_EOF, ""};
}

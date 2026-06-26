#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
  const char *src;
  int pos;
} Lexer;

void lexer_init(Lexer *lexer, const char *src);
Token lexer_next(Lexer *lexer);

#endif

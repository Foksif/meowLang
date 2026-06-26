#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static void next(Parser *p) { p->current = lexer_next(&p->lexer); }

static void expect(Parser *p, TokenType type) {
  if (p->current.type != type) {
    printf("parse error\n");
    exit(1);
  }
  next(p);
}

void parser_init(Parser *p, const char *src) {
  lexer_init(&p->lexer, src);
  next(p);
}

FunctionNode parse_function(Parser *p) {
  FunctionNode fn = {0};

  if (p->current.type == TOKEN_AT_APP) {
    fn.is_app = 1;
    next(p);
  }

  expect(p, TOKEN_FUNC);

  fn.name = p->current.lexeme;
  expect(p, TOKEN_IDENTIFIER);

  expect(p, TOKEN_LPAREN);
  expect(p, TOKEN_RPAREN);

  expect(p, TOKEN_ARROW);

  expect(p, TOKEN_IDENTIFIER); // int

  expect(p, TOKEN_LBRACE);

  expect(p, TOKEN_RETURN);

  fn.return_value = atoi(p->current.lexeme);
  expect(p, TOKEN_NUMBER);

  expect(p, TOKEN_SEMICOLON);

  expect(p, TOKEN_RBRACE);

  return fn;
}

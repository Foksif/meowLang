#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static void next(Parser *p) { p->current = lexer_next(&p->lexer); }

static const char *token_name(TokenType t) {
  switch (t) {
  case TOKEN_FUNC:
    return "func";
  case TOKEN_RETURN:
    return "return";
  case TOKEN_IDENTIFIER:
    return "identifier";
  case TOKEN_NUMBER:
    return "number";
  case TOKEN_LBRACE:
    return "{";
  case TOKEN_RBRACE:
    return "}";
  case TOKEN_LPAREN:
    return "(";
  case TOKEN_RPAREN:
    return ")";
  case TOKEN_ARROW:
    return "->";
  case TOKEN_SEMICOLON:
    return ";";
  case TOKEN_PLUS:
    return "+";
  case TOKEN_MINUS:
    return "-";
  case TOKEN_STAR:
    return "*";
  case TOKEN_SLASH:
    return "/";
  case TOKEN_AT_APP:
    return "@app";
  default:
    return "unknown";
  }
}

static void expect(Parser *p, TokenType type) {
  if (p->current.type != type) {
    printf("parse error: expected %s but got %s ('%s')\n", token_name(type),
           token_name(p->current.type), p->current.lexeme);
    exit(1);
  }
  next(p);
}

void parser_init(Parser *p, const char *src) {
  lexer_init(&p->lexer, src);
  next(p);
}

// STATEMENTS

Stmt *parse_statement(Parser *p) {
  if (p->current.type == TOKEN_RETURN) {
    next(p);

    Stmt *s = malloc(sizeof(Stmt));
    s->type = STMT_RETURN;
    s->expr = parse_expression(p);

    expect(p, TOKEN_SEMICOLON);
    return s;
  }

  printf("unknown statement\n");
  exit(1);
}

// FUNCTION

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
  expect(p, TOKEN_IDENTIFIER);

  expect(p, TOKEN_LBRACE);

  fn.body = NULL;
  fn.body_count = 0;

  while (p->current.type != TOKEN_RBRACE) {
    Stmt *s = parse_statement(p);

    fn.body = realloc(fn.body, sizeof(Stmt *) * (fn.body_count + 1));
    fn.body[fn.body_count++] = s;
  }

  expect(p, TOKEN_RBRACE);

  return fn;
}

// EXPRESSIONS

Expr *parse_factor(Parser *p) {
  Expr *e = malloc(sizeof(Expr));

  if (p->current.type == TOKEN_NUMBER) {
    e->type = EXPR_NUMBER;
    e->value = atoi(p->current.lexeme);
    e->left = e->right = NULL;

    next(p);
    return e;
  }

  if (p->current.type == TOKEN_LPAREN) {
    next(p);
    Expr *inside = parse_expression(p);
    expect(p, TOKEN_RPAREN);
    return inside;
  }

  printf("factor error\n");
  exit(1);
}

Expr *parse_term(Parser *p) {
  Expr *left = parse_factor(p);

  while (p->current.type == TOKEN_STAR || p->current.type == TOKEN_SLASH) {

    char op = (p->current.type == TOKEN_STAR) ? '*' : '/';
    next(p);

    Expr *right = parse_factor(p);

    Expr *node = malloc(sizeof(Expr));
    node->type = EXPR_BINARY;
    node->op = op;
    node->left = left;
    node->right = right;

    left = node;
  }

  return left;
}

Expr *parse_expression(Parser *p) {
  Expr *left = parse_term(p);

  while (p->current.type == TOKEN_PLUS || p->current.type == TOKEN_MINUS) {

    char op = (p->current.type == TOKEN_PLUS) ? '+' : '-';
    next(p);

    Expr *right = parse_term(p);

    Expr *node = malloc(sizeof(Expr));
    node->type = EXPR_BINARY;
    node->op = op;
    node->left = left;
    node->right = right;

    left = node;
  }

  return left;
}

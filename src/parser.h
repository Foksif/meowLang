#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

typedef struct {
  Lexer lexer;
  Token current;
} Parser;

void parser_init(Parser *parser, const char *src);

FunctionNode parse_function(Parser *parser);

Expr *parse_expression(Parser *parser);
Expr *parse_term(Parser *parser);
Expr *parse_factor(Parser *parser);

Stmt *parse_statement(Parser *parser);

#endif

#ifndef AST_H
#define AST_H

// EXPRESSIONS

typedef enum { EXPR_NUMBER, EXPR_BINARY } ExprType;

typedef struct Expr Expr;

struct Expr {
  ExprType type;

  int value; // number

  char op; // + - * /

  Expr *left;
  Expr *right;
};

// STATEMENTS

typedef enum { STMT_RETURN } StmtType;

typedef struct Stmt Stmt;

struct Stmt {
  StmtType type;
  Expr *expr;
};

// FUNCTION

typedef struct {
  char *name;

  Stmt **body;
  int body_count;

  int is_app;
} FunctionNode;

#endif

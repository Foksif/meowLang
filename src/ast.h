#ifndef AST_H
#define AST_H

typedef enum { EXPR_NUMBER, EXPR_BINARY, EXPR_VAR } ExprType;

typedef struct Expr Expr;

struct Expr {
  ExprType type;

  int value;

  char op;

  Expr *left;
  Expr *right;

  char *var_name; // ONLY FOR EXPR_VAR
};

// STATEMENTS
typedef enum { STMT_RETURN, STMT_VAR } StmtType;

typedef struct Stmt Stmt;

struct Stmt {
  StmtType type;

  Expr *expr; // return

  char *var_name; // var decl
  Expr *var_value;
};

// FUNCTION
typedef struct {
  char *name;

  Stmt **body;
  int body_count;

  int is_app;
} FunctionNode;

#endif

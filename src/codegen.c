#include "codegen.h"

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================
// SYMBOL TABLE
// =====================

typedef struct {
  char *name;
  LLVMValueRef ptr; // alloca pointer
} Var;

#define MAX_VARS 256

static Var vars[MAX_VARS];
static int var_count = 0;

// =====================
// EXPRESSIONS
// =====================

static LLVMValueRef codegen_expr(Expr *e, LLVMBuilderRef b) {

  // number
  if (e->type == EXPR_NUMBER)
    return LLVMConstInt(LLVMInt32Type(), e->value, 0);

  // variable (IMPORTANT)
  if (e->type == EXPR_VAR) {
    for (int i = 0; i < var_count; i++) {
      if (strcmp(vars[i].name, e->var_name) == 0) {
        return LLVMBuildLoad2(b, LLVMInt32Type(), vars[i].ptr, "loadtmp");
      }
    }

    printf("undefined variable %s\n", e->var_name);
    exit(1);
  }
  // binary
  LLVMValueRef L = codegen_expr(e->left, b);
  LLVMValueRef R = codegen_expr(e->right, b);

  switch (e->op) {
  case '+':
    return LLVMBuildAdd(b, L, R, "add");
  case '-':
    return LLVMBuildSub(b, L, R, "sub");
  case '*':
    return LLVMBuildMul(b, L, R, "mul");
  case '/':
    return LLVMBuildSDiv(b, L, R, "div");
  }

  printf("bad operator\n");
  exit(1);
}

// =====================
// MAIN CODEGEN
// =====================

void codegen(FunctionNode fn, const char *output) {

  LLVMModuleRef module = LLVMModuleCreateWithName("meow");

  LLVMTypeRef fnType = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);

  LLVMValueRef func = LLVMAddFunction(module, fn.name, fnType);

  LLVMBasicBlockRef block = LLVMAppendBasicBlock(func, "entry");

  LLVMBuilderRef builder = LLVMCreateBuilder();

  LLVMPositionBuilderAtEnd(builder, block);

  LLVMValueRef ret = NULL;

  var_count = 0;

  // =====================
  // STATEMENTS
  // =====================

  for (int i = 0; i < fn.body_count; i++) {

    Stmt *s = fn.body[i];

    // return
    if (s->type == STMT_RETURN) {
      ret = codegen_expr(s->expr, builder);
      continue;
    }

    // variable declaration
    if (s->type == STMT_VAR) {

      LLVMValueRef alloca =
          LLVMBuildAlloca(builder, LLVMInt32Type(), s->var_name);

      LLVMValueRef val = codegen_expr(s->var_value, builder);

      LLVMBuildStore(builder, val, alloca);

      vars[var_count++] = (Var){.name = s->var_name, .ptr = alloca};

      continue;
    }
  }

  if (!ret) {
    printf("no return statement\n");
    exit(1);
  }

  LLVMBuildRet(builder, ret);

  // =====================
  // VERIFY + OUTPUT
  // =====================

  if (LLVMVerifyModule(module, LLVMAbortProcessAction, NULL)) {
    printf("llvm verify failed\n");
    exit(1);
  }

  if (LLVMPrintModuleToFile(module, output, NULL)) {
    printf("failed to write llvm ir\n");
    exit(1);
  }

  printf("generated %s\n", output);

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "clang %s -o app", output);
  system(cmd);

  printf("built ./app\n");

  LLVMDisposeBuilder(builder);
  LLVMDisposeModule(module);
}

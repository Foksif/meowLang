#include "codegen.h"

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>

#include <stdio.h>
#include <stdlib.h>

static LLVMValueRef codegen_expr(Expr *e, LLVMBuilderRef b) {

  if (e->type == EXPR_NUMBER)
    return LLVMConstInt(LLVMInt32Type(), e->value, 0);

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

  printf("bad op\n");
  exit(1);
}

void codegen(FunctionNode fn, const char *output) {

  LLVMModuleRef module = LLVMModuleCreateWithName("meow");

  LLVMTypeRef fnType = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);

  LLVMValueRef func = LLVMAddFunction(module, fn.name, fnType);

  LLVMBasicBlockRef block = LLVMAppendBasicBlock(func, "entry");

  LLVMBuilderRef builder = LLVMCreateBuilder();

  LLVMPositionBuilderAtEnd(builder, block);

  LLVMValueRef ret = NULL;

  for (int i = 0; i < fn.body_count; i++) {
    Stmt *s = fn.body[i];

    if (s->type == STMT_RETURN) {
      ret = codegen_expr(s->expr, builder);
      break;
    }
  }

  if (!ret) {
    printf("no return\n");
    exit(1);
  }

  LLVMBuildRet(builder, ret);

  LLVMVerifyModule(module, LLVMAbortProcessAction, NULL);
  LLVMPrintModuleToFile(module, output, NULL);

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "clang %s -o app", output);
  system(cmd);

  LLVMDisposeBuilder(builder);
  LLVMDisposeModule(module);
}

#include "codegen.h"

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void codegen(FunctionNode fn, const char *output) {
  LLVMModuleRef module = LLVMModuleCreateWithName("meow");

  LLVMTypeRef fnType = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);

  LLVMValueRef function = LLVMAddFunction(module, fn.name, fnType);

  LLVMBasicBlockRef block = LLVMAppendBasicBlock(function, "entry");

  LLVMBuilderRef builder = LLVMCreateBuilder();

  LLVMPositionBuilderAtEnd(builder, block);

  LLVMBuildRet(builder, LLVMConstInt(LLVMInt32Type(), fn.return_value, 0));

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

  int result = system(cmd);

  if (result != 0) {
    printf("link failed\n");
    exit(1);
  }

  printf("built executable: ./app\n");

  LLVMDisposeBuilder(builder);
  LLVMDisposeModule(module);
}

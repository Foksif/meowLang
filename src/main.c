#include "codegen.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

static char *read_file(const char *path) {
  FILE *f = fopen(path, "r");

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  char *buffer = malloc(size + 1);
  size_t read = fread(buffer, 1, size, f);
  if (read != size) {
    printf("read error\n");
    exit(1);
  }
  buffer[size] = 0;

  fclose(f);

  return buffer;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: meowc file.meow\n");
    return 1;
  }

  char *source = read_file(argv[1]);

  Parser parser;
  parser_init(&parser, source);

  FunctionNode fn = parse_function(&parser);

  codegen(fn, "out.ll");

  printf("generated out.ll\n");

  return 0;
}

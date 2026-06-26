{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.llvm
    pkgs.clang
    pkgs.cmake
    pkgs.gcc
  ];
}

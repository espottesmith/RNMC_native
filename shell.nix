with (import <nixpkgs> {});

mkShell rec {
  buildInputs = [ clang
                  gcc
                  gsl
                  lldb
                  gdb
                  valgrind
                ];

  CPATH = lib.makeSearchPathOutput "dev" "include" [ clang.libc gsl ];
}

with (import /home/danielbarter/nixpkgs {});

let pythonEnv = python38.withPackages (
      ps: [ ps.pymatgen
            ps.numpy
            ps.openbabel-bindings
            ps.networkx
            ps.pygraphviz
          ]);
in mkShell rec {
  buildInputs = [ clang
                  gcc
                  gsl
                  lldb
                  gdb
                  valgrind
                  pythonEnv
                  graphviz
                  texlive.combined.scheme-small
                ];

  CPATH = lib.makeSearchPathOutput "dev" "include" [ clang.libc gsl ];
}

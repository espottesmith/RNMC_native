with (import /home/danielbarter/nixpkgs {});

let pythonEnv = python38.withPackages (
      ps: [ ps.pymatgen
            ps.numpy
            ps.openbabel-bindings]);
in mkShell rec {
  buildInputs = [ clang
                  gsl
                  lldb
                  gdb
                  valgrind
                  pythonEnv
                ];

  CPATH = lib.makeSearchPathOutput "dev" "include" [ clang.libc gsl ];
}

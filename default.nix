with import <nixpkgs> {}; {
  hawkEnv = stdenv.mkDerivation {
    name = "hawk";
    shellHook = ''
      export PATH="$PATH:$PWD/build/bin";
      '';
      
    
    buildInputs = [ stdenv cmake pkgconfig bison flex llvm boost libyamlcpp ];
  };
}
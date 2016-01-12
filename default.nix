with import <nixpkgs> {}; {
  hawkEnv = stdenv.mkDerivation {
    name = "hawk";
    buildInputs = [ stdenv cmake pkgconfig bison flex llvm boost libyamlcpp ];
  };
}

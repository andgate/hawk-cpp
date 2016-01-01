with import <nixpkgs> {}; {
  hawkEnv = stdenv.mkDerivation {
    name = "hawk";
    buildInputs = [ stdenv cmake pkgconfig llvm clang libyamlcpp gcc boost ];
  };
}

{
  description = "Trabalho de compiladores";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let pkgs = import nixpkgs { inherit system; };
    in {
      devShell = pkgs.mkShell {
        buildInputs = with pkgs; [
          clang
          cmake
        ];

        packages = with pkgs; [
          clang-tools
        ];
      };
    });
}

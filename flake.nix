{
  description = "Trabalho de compiladores";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = true;
        };
    in {
      defaultPackage = pkgs.stdenv.mkDerivation {
        pname = "compilerFront";
        version = "0.0.1";
        src = ./.;

        nativeBuildInputs = with pkgs; [ cmake ];

        meta =   {
          description = "Trabalho de compiladores";
          mainProgram = "compilerFront";
        };

        installPhase = ''
          mkdir -p $out/bin
          cp ./compilerFront $out/bin/
        '';
      };

      devShell = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          clang
          cmake
        ];

        packages = with pkgs; [
          clang-tools
          lldb

          (vscode-with-extensions.override {
            vscode = vscodium;
            vscodeExtensions = with vscode-extensions; [
              tuttieee.emacs-mcx
              llvm-vs-code-extensions.vscode-clangd
              ms-vscode.cmake-tools
              vadimcn.vscode-lldb
              dracula-theme.theme-dracula
              ms-ceintl.vscode-language-pack-pt-br
            ];
          })
        ];
      };
    });
}

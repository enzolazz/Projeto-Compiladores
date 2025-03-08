{
  description = "Trabalho de compiladores";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = true;
        };
    in {
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

# Dependências que devem ser instaladas
- CMake
- Algum compilador de C++ (gcc, clang, MSVC, ...)
- Algum Build system (make, ninja, ...)

Pode-se instalar todas as depências automaticamente usando [Nix](https://nixos.org/) através do comando abaixo:
```shell
# Assumindo que o PWD é a raíz do projeto
nix develop
```

# Compilar o projeto
Se você está usando uma IDE, compile através da IDE, mas caso contrário...

```shell
# Gerar arquivos do build system
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release # ou Debug

# Compilar
cmake --build build # build é o nome do diretório especificado no comando anterior
```

Para executar o programa:
```shell
./build/compilerFront
```
# c-starter-kit

Spits out a barebones C project. C99 and C23 (C2x).

```sh
./cstarter.sh myproj

./cstarter.sh --name myproj --template c2x-starter --output ~/src
./cstarter.sh --name myproj --standard c23 --layout birch
./cstarter.sh --list
```

Project names have to be C identifiers: `[A-Za-z_][A-Za-z0-9_]*`.

## Templates

| ID | Standard | Layout |
| --- | --- | --- |
| `c99-starter` | C99 | CMake (`src/` + `include/`) |
| `c2x-starter` | C23 | CMake (`src/` + `include/`) |
| `c99-birch` | C99 | Flat Makefile |
| `c2x-birch` | C23 | Flat Makefile |

`c2x-birch-starter` still works as an alias for `c2x-birch`.

Generated projects also get `.editorconfig` and `.clang-format`.

## After generation

CMake templates:

```sh
cd myproj
cmake -S . -B build
cmake --build build
```

Birch templates:

```sh
cd myproj
make
./myproj
```

C23 note: GCC 13 still wants `-std=c2x`. The birch Makefile sniffs that.

# c-starter-kit

Spits out a barebones C project. C99 and C23 (C2x).

```sh
./cstarter.sh myproj

./cstarter.sh --name myproj --template c2x-starter --output ~/src
./cstarter.sh --name myproj --standard c23
./cstarter.sh --list
./cstarter.sh --gui
```

Project names have to be C identifiers: `[A-Za-z_][A-Za-z0-9_]*`.

## Templates

| ID | Standard | Layout |
| --- | --- | --- |
| `c99-starter` | C99 | CMake (`src/` + `include/`) |
| `c2x-starter` | C23 | CMake (`src/` + `include/`) |

Generated projects also get `.editorconfig` and `.clang-format`.

## After generation

```sh
cd myproj
cmake -S . -B build
cmake --build build
```

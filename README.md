# c-starter-kit

Spits out a barebones C project. C99 and C23.

```sh
./cstarter.sh myproj

./cstarter.sh --name myproj --standard c23 --output ~/src
./cstarter.sh --list
./cstarter.sh --gui
```

Project names have to be C identifiers: `[A-Za-z_][A-Za-z0-9_]*`.

## Standards

| `--standard` | CMake tree |
| --- | --- |
| `c99` | `src/` + `include/`, C99 |
| `c23` | same layout, C23 |

Generated projects also get `.editorconfig` and `.clang-format`.

## Clay GUI

```sh
cmake -S gui -B gui/build
cmake --build gui/build
./cstarter.sh --gui
```

Needs raylib 5.x, or let CMake fetch 5.5. `--gui` will try to build it the first time.

Enter generates, Tab switches fields, 1-2 pick a template. The window is split (sidebar / form / preview) so extra options can land later without a rewrite.

## After generation

```sh
cd myproj
cmake -S . -B build
cmake --build build
```

# Feature overview

c-starter-kit drops a barebones C project on disk so I can skip the usual CMake / Makefile / header boilerplate.

Two axes:

- **Standard:** C99 or C23 (C2x in the template names, because that is still what a lot of toolchains call it)
- **Layout:** CMake starter (what is already in the repo) or a flat Birch-style Makefile tree

The generator is `cstarter.sh`. Optional native GUI is Clay + raylib and just shells out to that script.

C style for generated code is still TBD. Do not bikeshed formatting in the templates until that is settled.

## Feature matrix

| Feature | C99 CMake (`c99-starter`) | C23 CMake (`c2x-starter`) | C99 Birch (`c99-birch`) | C23 Birch (`c2x-birch`) |
| --- | --- | --- | --- | --- |
| Interactive template picker | yes | yes | yes | yes |
| Non-interactive `--template` | yes | yes | yes | yes |
| `--standard` + `--layout` | cmake | cmake | birch | birch |
| `PROJECTNAME` rename in paths + file contents | yes | yes | yes | yes |
| Copies `.editorconfig` / `.clang-format` | yes | yes | yes | yes |
| CMake executable target | yes | yes | no | no |
| GNU/Clang vs MSVC flag split | yes | yes | n/a | n/a |
| `c_std_99` / `c_std_23` | 99 | 23 (MSVC via `CMAKE_C_STANDARD`) | `-std=c99` | `-std=c23` with c2x fallback |
| Flat `Makefile` + one `.c` / `.h` | no | no | yes | yes |
| Clay GUI can pick it | yes | yes | yes | yes |

## Out of scope for now

- Picking / enforcing a C style guide
- Package managers, vendored third-party libs in generated projects
- CI for generated projects
- Installers

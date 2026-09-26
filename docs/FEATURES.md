# Feature overview

c-starter-kit drops a barebones C project on disk so I can skip the usual CMake / header boilerplate.

Two templates:

- **C99** (`c99-starter`) — CMake, `src/` + `include/`
- **C23** (`c23-starter`) — same layout, C23 compile flags (MSVC vs GCC/Clang handled in CMake)

The generator is `cstarter.sh`. Optional native GUI is Clay + raylib and just shells out to that script.

C style for generated code is still TBD. Do not bikeshed formatting in the templates until that is settled.

## Feature matrix

| Feature | C99 (`c99-starter`) | C23 (`c23-starter`) |
| --- | --- | --- |
| Interactive `--standard` picker | yes | yes |
| Non-interactive `--standard c99` / `--standard c23` | yes | yes |
| `PROJECTNAME` rename in paths + file contents | yes | yes |
| Copies `.editorconfig` / `.clang-format` | yes | yes |
| CMake executable target | yes | yes |
| GNU/Clang vs MSVC flag split | yes | yes |
| Language standard | `c_std_99` | `c_std_23` (MSVC via `CMAKE_C_STANDARD`) |
| Clay GUI can pick it | planned | planned |

## Out of scope for now

- Picking / enforcing a C style guide
- Choosing the compiler at generate time
- Package managers, vendored third-party libs in generated projects
- CI for generated projects
- Installers

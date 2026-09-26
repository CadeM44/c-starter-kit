# Plan

Work is split so each PR is one chunk. Style guide is still undecided; templates stay close to what is already on `main`.

## Done

- CLI flags, name checks, `--list`, `--standard`

## Next

- `feat/clay-gui` — Clay + raylib GUI that calls `cstarter.sh`

## Notes

- Keep existing `c99-starter` and `c23-starter` CMake files. They already handle the MSVC vs GCC/Clang C23 mess.
- GUI is C99 + Clay. Raylib is the renderer.
- Generated project names stay C identifiers so the substituted symbols compile.

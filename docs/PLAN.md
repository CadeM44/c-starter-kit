# Plan

Work is split so each PR is one chunk. Style guide is still undecided; templates stay close to what is already on `main`.

## Branch / PR order

1. `docs/feature-plan` — this overview + matrix + plan
2. `feat/cli` — flags, name checks, `--list`, `--standard`
3. `feat/clay-gui` — Clay + raylib GUI that calls `cstarter.sh`

Merge 2 before 3. 3 depends on the CLI flags existing.

## Notes

- Keep existing `c99-starter` and `c2x-starter` CMake files. They already handle the MSVC vs GCC/Clang C23 mess.
- GUI is C99 + Clay. Raylib is the renderer.
- Generated project names stay C identifiers so the substituted symbols compile.

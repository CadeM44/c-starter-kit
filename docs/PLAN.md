# Plan

Work is split so each PR is one chunk. Style guide is still undecided; templates stay close to what is already on `main`.

## Branch / PR order

1. `docs/feature-plan` — this overview + matrix + plan
2. `feat/birch-templates` — flat Makefile starters (`c99-birch`, `c2x-birch`)
3. `feat/cli` — flags, name checks, `--list`, `--standard` / `--layout`
4. `feat/clay-gui` — Clay + raylib GUI that calls `cstarter.sh`

Merge 2 and 3 before 4 if possible. 4 depends on the CLI flags existing.

## Notes

- Keep existing `c99-starter` and `c2x-starter` CMake files. They already handle the MSVC vs GCC/Clang C23 mess.
- `c2x-birch-starter` stays as an alias for `c2x-birch` because the script already special-cased that name.
- GUI is C99 + Clay. Raylib is the renderer. No extra Python UI in the repo.
- Generated project names stay C identifiers so the substituted symbols compile.

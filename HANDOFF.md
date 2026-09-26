# Handoff — c-starter-kit Clay GUI

Repo: `CadeM44/c-starter-kit`  
Branch: `feat/clay-gui`  
PR: #5

## What this is

A tiny kit that writes a barebones C project to disk so you skip CMake / header boilerplate.

- **CLI:** `cstarter.sh` — `--name`, `--standard c99|c23`, `--output`, `--list`, `--gui`
- **Templates:** `templates/c99-starter`, `templates/c23-starter` (CMake, `src/` + `include/`, `PROJECTNAME` placeholders)
- **GUI:** Clay + raylib. It does not generate files itself. It shells out to `cstarter.sh --name … --standard … --output …`

C style kit-wide: Allman braces, 4-space indent, PascalCase types, snake_case functions, module prefixes (`App_`, `UI_`). `.clang-format` / `.editorconfig` match and get copied into generated trees. Do not copy a CONVENTIONS.md into generated projects.

## Repo map

```
cstarter.sh              generator + --gui launcher
templates/c99-starter/   hello-world C99 CMake tree
templates/c23-starter/   same layout, C23 / MSVC C23 flags
gui/
  theme.h                muted 1977 Apple rainbow on cream
  app.h / app.c          state, keys, generate command
  layout.h / layout.c    Clay widget tree
  main.c                 raylib window + Clay frame loop
  CMakeLists.txt         fetches clay + raylib 5.5 if missing
README.md
HANDOFF.md               this file
```

## How the GUI is split

- `App_Page` / sidebar exist so later pages (options, settings) are add-ons, not a rewrite
- `App_Template` rows: C99 and C23. Keys: Tab fields, Enter generate, 1-2 pick standard
- `app_generate()` builds:
  `bash "<kit>/cstarter.sh" --name "<id>" --standard "c99|c23" --output "<dir>"`
- Theme colors are brace-initialized (`static const Clay_Color`) because MSVC rejects compound literals as constant initializers (`C2099`)

## Where we left off

Cade is on Windows (PowerShell + MSVC + VSCode). The GUI **compiles** after the MSVC fixes. Running it through `cstarter.sh` does not.

### Already fixed on this branch

1. MSVC `unistd.h` — `app.c` / `main.c` use `_access` / `_getcwd` / `_popen` / `_pclose` on `_WIN32`
2. MSVC `C2099` — `gui/theme.h` no longer uses `UI_COL(...)` in static initializers
3. Windows `find.exe` hang — `apply_placeholders` used GNU `find`. PATH resolves to `C:\Windows\System32\find.exe`, which reads stdin and never returns. Debugger sat on `while IFS= read -r path`. Placeholder rewrite is now a bash directory walk; no `find`.
4. `--gui` did not see `gui/build/Debug/cstarter-gui.exe` (MSVC name). Launcher now checks `.exe` under Debug/Release/build.

### Current blocker (unfixed)

`--gui` still “hangs.” VSCode now stops on:

```bash
is_ident() {
    [[ "$1" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]
}
```

That function only runs on the **generate** path, not `launch_gui`. So either:

- the debug session is not passing `--gui` (script falls through to interactive generate / name check), or
- the GUI did start and Generate spawned a child `cstarter.sh`, or
- Git Bash + the VSCode bash debugger is choking on `[[ =~ ]]` (common; the regex itself is fine in a normal terminal)

`is_ident` should be rewritten without `=~` anyway (character loop), both for Windows and so a debugger cannot sit on that builtin.

Quick isolate, no debugger:

```powershell
# 1. Does the exe run at all?
.\gui\build\Debug\cstarter-gui.exe

# 2. Does the script launch it?
.\cstarter.sh --gui

# 3. Does generate work without the GUI?
.\cstarter.sh --name smoke_c99 --standard c99 --output $env:TEMP
```

If (1) opens a window, the script/debugger is the problem, not Clay. If (3) hangs on `is_ident`, replace the regex before anything else.

## What’s next (in order)

1. **Unstick Windows launch**
   - Replace `is_ident` `=~` with a C-style character scan
   - Confirm `--gui` execs `cstarter-gui.exe` and that Generate’s child script finishes
   - Watch for the next Windows-PATH trap (`sort.exe`, `find.exe`, `bash` missing from the GUI’s `popen` command)
2. **Manual smoke** (merge bar for PR #5) — build GUI, C99 + C23 generate from GUI, invalid name rejected, existing dir rejected, generated tree builds
3. **PR hygiene** — GitHub still reports a merge conflict vs `main` on `templates/c23-starter/src/PROJECTNAME.c` (tabs vs 4-space Allman). Resolve in the GitHub UI; keep 4-space.
4. **Only after the GUI actually runs** — extra `App_Page`s / generate-time toggles. Do not rewrite the layout for that.

Out of scope until the above is boring: compiler picker, package managers, CI for generated projects, installers.

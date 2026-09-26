# C Style & Conventions

> This document defines formatting, naming, and source organization conventions.
>
> It does **not** define which C language features or programming techniques are permitted.

Machine-readable copy: `.clang-format` and `.editorconfig` in the kit root. Both are copied into generated projects.

## Formatting

### Indentation

Use indentation equivalent to **4 spaces per level**.

### Braces

Use Allman-style braces. Apply the same brace style to functions, structs, enums, unions, and other braced constructs.

### Control Statements

Include a space between a control-flow keyword and its opening parenthesis.

### Function Definitions

Keep the return type, function name, and parameter list on the same line when practical.

### Pointers

Attach `*` to the variable or declarator (`NET_Connection *connection`).

### Declarations

Prefer one variable per declaration. Struct members should be declared individually.

### Line Length

Soft guideline only. Do not wrap a self-contained declaration solely to hit a column limit. Long expressions inside function logic should be broken up when that improves readability.

# Naming

- `PascalCase` for types
- `snake_case` for functions, variables, parameters, and struct members
- Lowercase `snake_case` for file and directory names
- Short module prefixes where namespacing is useful (`NET_Connection`, `net_connection_open`)
- Foundational types may be plain `PascalCase` (`String`, `Arena`)
- Do not use `_t` suffixes for project-owned types
- Enum members begin with the full enum type name; use `_COUNT` as a sentinel when appropriate
- Prefer `module_subject_action` or `module_result_from_input`
- Avoid terse primitive aliases (`U32`, `F32`, and friends)

# Files and Folders

Names follow responsibility or subsystem (`network/`, `renderer/`). Avoid generic buckets (`misc/`, `helpers/`, `types/`) unless they are a real shared concept.

# Source Organization

Organize around modules, not language constructs. Types live with the module that owns them.

A `.c` file includes its matching header first, then the standard library, then third-party headers, then other project headers.

Keep a symbol private to the `.c` file unless another module needs it.

# Platform and External APIs

External APIs keep their native names. Project-owned wrappers around them follow these conventions.

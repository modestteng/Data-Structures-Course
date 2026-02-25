# Project Agent Rules

- When generating or modifying code, add concise block-by-block comments to improve readability for beginners.
- Keep behavior unchanged unless the user explicitly asks for logic refactoring.
- Prefer comments above major sections: setup, core logic, edge handling, output, and cleanup.
- Avoid noisy line-by-line comments for trivial statements.
- Match comment language to the user's request language.
- In this project, all code must use C language only.
- Use fixed comment prefixes for consistency:
- `// Block:` at the start of each major code block.
- `// Idea:` at the end of a block only when a clear engineering or philosophical principle is present.

## Naming Convention

- All file names, folder names, variable names, function names, struct names, and any other identifiers must be in English only.
- No Chinese characters are allowed in file names or folder names.
- Code identifiers (variables, functions, structs, macros) must be in English.
- Comments inside source code files may be written in Chinese.
- When creating new files or folders, use lowercase English words separated by underscores (snake_case).

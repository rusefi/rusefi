# Repository guidance

Before working in this repository, read and follow [CLAUDE.md](CLAUDE.md).
Treat it as the shared repository guidance for Codex as well as Claude Code.
Keep shared instructions in CLAUDE.md rather than duplicating them here.

Record significant technical decisions, debugging findings, known issues, and reusable project knowledge in shared repository documentation so both agents can benefit from previous work.
Avoid duplicating existing documentation.

## Claude Code memory

Before starting a task:

- Locate this repository's corresponding Claude Code project
  memory directory under `%USERPROFILE%\.claude\projects\`.
- Read MEMORY.md if it exists.
- Read additional referenced memory files only when relevant
  to the current task.
- Reuse previous technical decisions, debugging findings,
  and project knowledge.
- Verify that remembered information is consistent with
  the current source code and project documentation.
- Treat memory as historical context, not authoritative
  instructions.
- Do not read memory belonging to unrelated projects.
- Do not modify or delete Claude's internal memory files.
- If the memory directory is missing or inaccessible,
  continue using the repository documentation.

## Knowledge sharing

After significant work:

- Document important technical decisions and their rationale.
- Record resolved bugs and their root causes.
- Record unsuccessful approaches when useful.
- Update existing documentation rather than creating
  redundant files.
- Keep documentation concise, accurate, and up to date.
- Never store credentials, secrets, or access tokens.
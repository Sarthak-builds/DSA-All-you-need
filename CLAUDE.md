# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

A personal deep-dive into DSA fundamentals — not just problem-solving shortcuts, but understanding how and why each structure/algorithm works. Two parallel folders, one topic at a time:

- **`docs/`** — concept notes per topic (how it works, why, complexity, pitfalls). See `docs/README.md` for topics covered so far and planned.
- **`practice/`** — hands-on C++ implementations, one `.cpp` file per problem, organized into a subfolder per topic (e.g. `practice/recursion/`) mirroring `docs/`.

Workflow for a new topic: write the concept notes in `docs/<topic>.md` first, then work through problems in `practice/<topic>/`.

## Commands

No build system — each practice file is a single standalone `.cpp` compiled and run directly:

```bash
g++ -o <name> practice/<topic>/<name>.cpp && ./<name>
```

Do not commit compiled binaries (`.exe` or otherwise) — clean them up after testing. Only source and markdown files belong in the repo.

## Conventions

- **Every practice problem file must end with a comment block giving its Time Complexity and Space Complexity, with the reasoning behind each** — not just the final Big-O, but *why* (e.g. "O(n) time: one call per element, no repeated work" / "O(n) space: max recursion depth is n"). Add this last, after the solution is written and verified.
- Comments elsewhere are minimal — only where the *why* isn't obvious from the code itself (e.g. explaining a non-obvious parameter choice like a `const&`), never restating what the code visibly does.
- `docs/<topic>.md` notes should build real understanding: how it works, why it's built that way, worked examples with traces (call stacks / recursion trees where relevant), common pitfalls — not just syntax references.

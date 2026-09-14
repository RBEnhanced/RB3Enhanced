# Contributing to RB3Enhanced

We welcome all contributions to help make RB3Enhanced a better mod, by improving
existing features or adding new ones.

## Codebase

* Visual Studio Code config files are included in the repository, so if you are
  using VSCode as your IDE, make sure it is following this file.
* For cross-platform compatibility reasons, the codebase aims to be C89/ANSI C.
  Variables must be defined at the start of functions rather than inline.
    * Xbox-specific implementations (must be guarded by `#ifdef RB3E_XBOX`) may
      be slightly non-compliant, as MSVC's C89 implementation is non-standard.
    * Wii or PS3-specific implementations (must be guarded by `#ifdef RB3E_WII`
      and/or `#ifdef RB3E_PS3`) may be compliant GNU C99.
* All indentations should be 4 spaces.
* Try to adhere to any existing code styling already used in the codebase.
* Function addresses should be added to the applicable
  `include/ports_{platform}.h` header file.
* When adding a function reference (whether as a hook or as a callee), make sure
  to add a unique stub in `source/_functions.c` and definitions of the original
  function in a header file in `include/rb3`.
* Please make sure all your contributions are able to be licensed under the
  GPLv2 or later. This means not using non-free or incompatibly licensed
  libraries or external code.

## Pull requests

* Test your changes before submitting them as a pull request, to ensure that
  they don't crash.
    * Emulators may mask fault behaviour, or return different results compared
    to a real console, so it's advised to use real hardware if possible.
    * Let us know what platform you've tested on.
* If possible, try to include port addresses for Xbox 360, Wii, PS3 and Bank 8
  in your contribution.
    * If you can't do this, guard the implementation behind
    `#ifdef RB3E_{PLATFORM}`/`#ifndef RB3E_{PLATFORM}`.
* It's recommended that you enable "Allow contributors to edit code" in your
  pull request, so quick fixes can be added by us before merging.
* Try to keep a clean Git history.
    * Don't make excessive commits and don't force push too often.
    * If your PR hasn't been merged, try to keep it up to date with changes
    in the `master` branch.

## LLM Policy

Responsible usage of LLMs to contribute code into RB3Enhanced is highly
discouraged, but allowed. When working on and submitting pull requests to
RB3Enhanced using Large Language Models (or "Generative AI"), please keep the
following in mind:

* **Purely "agentic" PRs with no human involvement are strictly forbidden and
    will be rejected. A human must be the one sending the PR.**
* When making the PR, you must at minimum disclose that LLMs were used in their
  creation, as well as the following:
    * Which model/service was used
    * How much the LLM was relied on (whether only to provide a few lines, or
      if it was entirely prompt driven / "vibe coded", for example)
    * **Trying to submit LLM code without disclosing will result in a ban.**
* LLMs can *not* be used to make large changes (e.g. "cleanups") to the codebase
  unprompted. The only changed code must be directly relevant to the PR.
* LLMs **must not** make tests. Tests written by LLMs are **strictly forbidden**
  as they will bloat the repo excessively.
* All text in the PR body must be human-written. Do not use an LLM to describe
  your changes.
* All user-facing text must be written by a human.
* You must ensure the code builds across all supported platforms and does the
  intended behaviour.
* You're expected to be able to point to any given line of code in your own
  change and explain why a certain decision was made - the answer should not be
  "IDK, the AI did it."
* LLMs are known to emit some amounts of code from copyrighted projects. If we
  suspect the output has enough copyrightable code from an incompatibly licensed
  project, we can't accept it.
* New files designed to assist with LLM development (e.g. `CLAUDE.md`,
  `AGENTS.md`, or any skills) will be rejected.

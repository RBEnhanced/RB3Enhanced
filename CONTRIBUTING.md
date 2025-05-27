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
    * The output of LLMs (e.g. ChatGPT) is not compatible with the GPLv2. Please
    don't use these to generate code when working on code in RB3Enhanced!

## Pull requests

* Test your changes before submitting them as a pull request, to ensure that
  they don't crash.
    * Emulators may mask fault behaviour, or return different results compared
    to a real console, so it's advised to use real hardware if possible.
    * Let us know what platform you've tested on.
* If possible, try to include port addresses for both Xbox 360, Wii, PS3
  and Bank 8 in your contribution.
    * If you can't do this, guard the implementation behind
    `#ifdef RB3E_{PLATFORM}`.
* It's recommended that you enable "Allow contributors to edit code" in your
  pull request, so quick fixes can be added by us before merging.
* Try to keep a clean Git history.
    * Don't make excessive commits and don't force push too often.
    * If your PR hasn't been merged, try to keep it up to date with changes
    in the `master` branch.

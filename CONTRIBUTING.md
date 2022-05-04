# Contributing to RB3Enhanced

We welcome all contributions to help make RB3Enhanced a better mod, by improving existing features or adding new ones.

## Codebase

* Visual Studio Code config files are included in the repository, so if you are using VSCode as your IDE, make sure it is following this file.
* For compatibility reasons, the codebase is C89 / ANSI C. Variables must be defined at the start of functions rather than inline.
* All indentations should be 4 spaces.
* Try to adhere to any existing code styling already used in the codebase.
* Function addresses should be added to `include/ports.h` in the respective platform section.
* When adding a function reference (whether as a hook or as a callee), make sure to add a unique stub in `source/_functions.c` and definitions of the original function in a header file in `include/rb3`.
* Please make sure all your contributions are able to be licensed under the GPLv2 or later. This means not using non-free or incompatibly licensed libraries or external code.

## Pull requests

* Test your changes on hardware before submitting them as a pull request, to ensure that they don't crash.
* If applicable, try to include port addresses for both Xbox 360 and Wii in your contribution.
* It's recommended that you enable "Allow contributors to edit code" in your pull request, so quick fixes can be added by us before merging.
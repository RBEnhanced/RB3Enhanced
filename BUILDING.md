# Building RB3Enhanced

RB3Enhanced is a cross-platform mod for both Xbox 360 and Wii. It's designed to be compiled on a Windows system, however the Wii version is able to compile on macOS and Linux.

## Prerequisites

* [devkitPPC](https://devkitpro.org/wiki/Getting_Started) (for Wii compilation)
* [BrainSlug](https://github.com/Chadderz121/brainslug-wii/blob/master/INSTALLING) (for Wii compilation)
* Xbox 360 SDK, with build tools installed (for Xbox compilation)
* Make \*, \*\*

You do not need devkitPPC to compile the mod for Xbox, and you do not need the Xbox 360 SDK to compile the mod for Wii.

The environment variable `XEDK` must be set to the install directory for the Xbox 360 SDK, and the environment variable `DEVKITPPC` must be set to the install directory for devkitPPC. The installers for the respective toolchains will automatically do this for you.

*\* There is a build script provided as `make_xbox.bat` that will compile using only the Xbox 360 SDK. This is not recommended.*

*\*\* On Windows, the only tested configuration is with Make installed via MSYS2. Any other configuration has not been tested, and may not work.*


## Compilation

Open a Terminal (or Command Prompt) window in the same folder you have cloned the source code into.

* To build for Xbox 360, type `make xbox`. A DLL file will be output at `out/RB3Enhanced.dll`, that can then be loaded with RB3ELoader or a modified Rock Band 3 XEX.
* To build for Wii, type `make wii`. A MOD file will be output at `out/RB3Enhanced.mod`, that can then be loaded with RB3ELoader or the BrainSlug channel.

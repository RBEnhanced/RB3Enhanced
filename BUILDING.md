# Building RB3Enhanced

RB3Enhanced is a cross-platform mod for both Xbox 360 and Wii. It's designed to be compiled on a Windows system, however both versions are able to compile on macOS and Linux.

## Prerequisites

* [devkitPPC](https://devkitpro.org/wiki/Getting_Started) (for Wii compilation)
* [BrainSlug](https://github.com/Chadderz121/brainslug-wii/blob/master/INSTALLING) (for Wii compilation)
* Xbox 360 SDK, with build tools installed (for Xbox compilation)
* Make \*, \*\*

You do not need devkitPPC to compile the mod for Xbox, and you do not need the Xbox 360 SDK to compile the mod for Wii.

The environment variable `XEDK` must be set to the install directory for the Xbox 360 SDK, and the environment variable `DEVKITPPC` must be set to the install directory for devkitPPC. The installers for the respective toolchains will automatically do this for you.

If compiling the Xbox 360 version under macOS or Linux, set the environment variable `WINDOWS_SHIM` to the path to your Wine executable. Note that some Wine versions may not work correctly with MSVC. You will also have to ensure `XEDK` is set to a path accessible under Wine, e.g. `Z:\Users\emma\xedk\`.

*\* There is a build script provided as `make_xbox.bat` that will compile using only the Xbox 360 SDK. This is not recommended.*

*\*\* On Windows, the only tested configuration is with Make installed via MSYS2. Any other configuration has not been tested, and may not work.*


## Compilation

Open a Terminal (or Command Prompt) window in the same folder you have cloned the source code into.

* To build for Xbox 360, type `make xbox`. A DLL file will be output at `out/RB3Enhanced.dll`, that can then be loaded with RB3ELoader or a modified Rock Band 3 XEX.
* To build for Wii, type `make wii`. A MOD file will be output at `out/RB3Enhanced.mod`, that can then be loaded with RB3ELoader or the BrainSlug channel.
* Typing `make` or `make all` will compile both. Adding `-jX` (where X is your processor's thread count) will speed up compliation times.

Adding `DEBUG=1` to the make command will build a version of RB3E that will output more debugging information to the logs.
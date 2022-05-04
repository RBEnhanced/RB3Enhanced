@echo off
setlocal enabledelayedexpansion
echo // This file will be auto generated during building, and should
echo // not be modified manually
echo.
echo // The git tag used to compile RB3Enhanced
git --version >nul 2>&1 && (
    FOR /F "tokens=* USEBACKQ" %%F IN (`git describe --always --tags --dirty`) DO (
        echo #define RB3E_BUILDTAG "%%F"
    )
    echo // The git branch and commit used to compile RB3Enhanced
    FOR /F "tokens=* USEBACKQ" %%F IN (`git describe --always --match "tag" --dirty`) DO (
        echo #define RB3E_BUILDCOMMIT "unknown-%%F"
    )
) || (
    echo #define RB3E_BUILDTAG "Unknown"
    echo // The git branch and commit used to compile RB3Enhanced
    echo #define RB3E_BUILDCOMMIT "%DATE% @ %COMPUTERNAME%"
)
endlocal
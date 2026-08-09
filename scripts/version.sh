if [ "$1" = "DEBUG" ]; then
    SUFF="-debug"
else
    SUFF=""
fi
if git --version &>/dev/null; then
# if git is installed, get the current information from the repository
    [[ ! -z ${GITHUB_ACTIONS} ]] && [[ ${RUNNER_OS} == "Linux" ]] && git config --global --add safe.directory /__w/RB3Enhanced/RB3Enhanced
    cat > source/version.h <<EOF
// This file will be auto generated during building, and should
// not be modified manually
// The git tag used to compile RB3Enhanced
#define RB3E_BUILDTAG "$(git describe --always --tags --dirty)$SUFF"
// The git branch and commit used to compile RB3Enhanced
#define RB3E_BUILDCOMMIT "$(git rev-parse --abbrev-ref HEAD)-$(git describe --always --match \"tag\" --dirty)$SUFF"
EOF
else
# if git isn't installed, we don't know the version, use the compile time in logs
    cat > source/version.h <<EOF
// This file will be auto generated during building, and should
// not be modified manually
// The git tag used to compile RB3Enhanced
#define RB3E_BUILDTAG "Unknown$SUFF"
// The git branch and commit used to compile RB3Enhanced
#define RB3E_BUILDCOMMIT "$(date) @ $HOSTNAME$SUFF"
EOF
fi
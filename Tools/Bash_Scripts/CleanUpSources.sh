#!/bin/bash
# Simple script to clean up source code (line endings, trailing spaces, ...)

# ==========================================================================
# Variable initialization
# ==========================================================================
declare -a SRC_PATHS=("src" "simulations")
#declare -a SRC_PATHS=(".")
declare -a FILE_EXTENSIONS=("*.c" "*.cc" "*.cpp" "*.h" ".hpp" "*.msg" "*.ned" "*.ini")

#declare -a SRC_PATHS=("../../src/Components/BasicBlocks")
#declare -a FILE_EXTENSIONS=("*.ned")

# ==========================================================================
# Functions
# ==========================================================================

# --------------------------------------------------------------------------
# Clean source directory
# --------------------------------------------------------------------------

function CleanSourceDir
{
    Path=$1
    Ext=$2

    echo "Searching for ${Ext} in ${Path}"
    echo ""

    echo "  Converting DOS encoding to UNIX encoding"
    find ${Path} -name ${Ext} ! -type d -exec bash -c 'dos2unix -q "$0"' {} \;

    echo "  Replacing tabs with 4 spaces"
    find ${Path} -name ${Ext} ! -type d -exec bash -c 'expand -t 4 "$0" > /tmp/e && mv /tmp/e "$0"' {} \;

    echo "  Removing trailing spaces"
    SED_CMD='sed '\''s/[[:space:]]*$//'\'' $0 > /tmp/e && mv /tmp/e $0'

    find ${Path} -name ${Ext} ! -type d -exec bash -c "${SED_CMD}" {} \;
}

# --------------------------------------------------------------------------
# main
# --------------------------------------------------------------------------
for SrcPath in "${SRC_PATHS[@]}"
do
    for FileExt in "${FILE_EXTENSIONS[@]}"
    do

        if [ "$#" -ne 1 ]; then
            echo "Illegal number of parameters. Root directory required."
            exit
        fi

        ROOT_PATH="$1"

        echo "Root: " ${ROOT_PATH}

        CleanSourceDir "${ROOT_PATH}/${SrcPath}" ${FileExt}

    done
done

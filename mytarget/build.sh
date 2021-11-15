#!/bin/bash
set -e

##
# Pre-requirements:
# - env TARGET: path to target work dir
# - env OUT: path to directory where artifacts are stored
##

if [ ! -d "$TARGET/repo" ]; then
    echo "fetch.sh must be executed first."
    exit 1
fi

# build mytarget executable
cd "$TARGET/repo"

#gcc -o mytarget_fuzz main.c

#cp mytarget_fuzz "$OUT/"


$CXX $CXXFLAGS -std=c++11 "$TARGET/repo/main.c"\
    -o $OUT/mytarget_fuzz -Wdiv-by-zero -Wno-deprecated -Wunused-comparison -Wno-error \
    $LDFLAGS $LIBS 

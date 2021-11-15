#!/bin/bash

##
# Pre-requirements:
# - env TARGET: path to target work dir
##

git clone --no-checkout https://github.com/slashsec-edu/fuzz-target.git "$TARGET/repo"
git -C "$TARGET/repo" checkout 29cbae5bad13609cb8b0c25a7ddf791ba44ab5e8



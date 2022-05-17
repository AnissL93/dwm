#!/usr/bin/env sh

# install dwm
make clean 2>&1 > /dev/null ; sudo make install 2>&1 > /dev/null && echo "Dwm installed!"

# install dwmblock
cd dwmblocks
make clean 2>&1 > /dev/null ; sudo make install 2>&1 > /dev/null && echo "Dwmblock installed!"

script_path=$(realpath ./scripts)
export PATH=$PATH:$script_path
echo "Add scripts $script_path to PATH"

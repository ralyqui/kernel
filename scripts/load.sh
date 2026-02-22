#!/bin/bash

make clean
rm -rf iso_root
rm image.hdd
rm image.iso

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

bear -- make && \
"$SCRIPT_DIR/create-iso.sh" && \
"$SCRIPT_DIR/mbr.sh" && \
qemu-system-x86_64 -d int,cpu_reset \
    -D qemu.log \
    -no-reboot \
    -drive format=raw,file=image.hdd -serial stdio

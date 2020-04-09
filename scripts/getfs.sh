#!/bin/bash

# Used to retrieve the file system within the os
# !!! Do NOT modify the size of fs/fs or bin/os
# after the build

OS=./bin/os
FS=./fs/fs
OUT=$FS
SECTOR=512

os_size=$(stat -c %s $OS)
fs_size=$(stat -c %s $FS)

os_without_fs=$(($os_size - $fs_size))
skip=$(($os_without_fs / $SECTOR))
count=$(($fs_size / $SECTOR))

dd if=$OS of=$OUT bs=$SECTOR skip=$skip count=$count

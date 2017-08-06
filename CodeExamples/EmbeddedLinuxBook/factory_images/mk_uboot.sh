#!/bin/bash

# Shell script for creating a U-boot script

if [ $# -eq 1 ]
  then
    DEST="/var/lib/tftpboot"
  else
    DEST="$2"
fi

mkimage -T script -C none -n 'boot script' -d $1 $1.scr
mv $1.scr $DEST

#!/bin/bash

# Shell script for creating a U-boot kernel image

mkimage -T kernel -O linux -A arm  -C none -a 30008000 -e 30008000 -n $1 -d $1 $1.um

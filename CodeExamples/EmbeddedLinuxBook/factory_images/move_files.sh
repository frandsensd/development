#! /bin/bash

#  Copy image files to the tftp directory.  If no parameter given, destination
#  is /var/lib/tftpboot

if [ $# -eq 0 ]
  then
    DEST="/var/lib/tftpboot"
  else
    DEST="$1"
fi

cp target_fs.yaffs $DEST
cp u-boot.bin $DEST
cp uImage-3.5.3 $DEST/uImage

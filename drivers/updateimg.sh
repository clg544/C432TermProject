#!/bin/bash
rmtcp download cpheader.bin
rmtcp download bootable.img
sudo dd if=cpheader.bin of=/dev/disk2 seek=0
sudo dd if=bootable.img of=/dev/disk2 seek=1 
exit

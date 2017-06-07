# script allows to upload straight via USB
rm firmware.bin
particle compile electron ./ --saveTo firmware.bin
particle flash --usb firmware.bin
particle serial monitor

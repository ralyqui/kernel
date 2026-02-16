make clean
rm -rf iso_root
rm image.hdd
rm image.iso

bear -- make && ./create-iso.sh && ./mbr.sh && qemu-system-x86_64 -drive format=raw,file=image.hdd -serial stdio

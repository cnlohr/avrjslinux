all :

everything : environment outputfiles

environment : 
#	wget --continue http://www.kernel.org/pub/linux/kernel/v2.6/linux-2.6.20.tar.gz
#	if [ ! -d linux-2.6.20 ]; then tar xzvf linux-2.6.20.tar.gz; fi
#	(cd linux-2.6.20; patch -t -p1 < ../kernelpatch; cp ../kernelconfig ./.config)
	wget --continue http://www.kernel.org/pub/linux/kernel/v2.6/linux-2.6.25.tar.bz2
	if [ ! -d linux-2.6.25 ]; then tar xjvf linux-2.6.25.tar.bz2; fi
	(cd linux-2.6.25; patch -t -p1 < ../kernelpatch; cp ../config25 ./.config)

linuxstart-20110820/vmlinux26.bin :
	(cd linuxstart-20110820; make kernel)

linuxstart-20110820/linuxstart.bin :
	(cd linuxstart-20110820; make)

outputfiles : linuxstart-20110820/linuxstart.bin linuxstart-20110820/vmlinux26.bin output/root.bin
	cp linuxstart-20110820/vmlinux26.bin output/vmlinux-2.6.20.bin
	cp linuxstart-20110820/linuxstart.bin output/

output/root.bin :
	dd if=/dev/zero of=$@ bs=1M count=4
	echo y | mkfs.ext2 $@
	sudo mount -o loop $@ rootfstemp
	sudo cp -a rootfs/* rootfstemp
	sync
	sudo umount rootfstemp

root : .
	sudo mount -o loop output/root.bin rootfstemp
	sudo cp -a rootfs/* rootfstemp
	sync
	df -h | grep rootfstemp
	sudo umount rootfstemp

mount : .
	sudo mount -o loop output/root.bin rootfstemp

unmount : .
	sudo umount rootfstemp

